#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <fstream>
const int POPULATION_SIZE = 200; // should be divisible by 4
const int GENERATIONS = 50;
int N; // number of items
int M; // MAX capacity of backpack
int global_mutations_count = 0;

double global_mutations_success = 0.0;
int __some_random__index = 0;
int kid_best_fitness = 0;
int kids = 0;
double avg_breakpoint = 0;
int crossover_amnt = 0;

auto t(std::chrono::system_clock::now().time_since_epoch().count());
std::mt19937 generator(t);
struct Item
{
    int weight = 0;
    int value = 0;
    Item() {}
    Item(int weight, int value) : weight(weight), value(value) {}
};

std::vector<Item> items;
struct Backpack
{
    int current_capacity = 0;
    double fitness = 0;
    std::vector<bool> inventory;
    Backpack() { inventory.resize(N); }
    bool operator<(const Backpack &k)
    {
        return this->fitness < k.fitness;
    }
    Backpack &operator=(const Backpack &rhs)
    {
        this->current_capacity = rhs.current_capacity;
        this->fitness = rhs.fitness;
        this->inventory = rhs.inventory;
        return *this;
    }
};
double mean(std::vector<double> &vec)
{
    double cum_sum = std::accumulate(vec.begin(), vec.end(), 0.0);
    int n = vec.size();
    return cum_sum / n;
}
double CDF(double lambda, double mu, double sigma)
{
    return 0.5 * (1 + erf((lambda - mu) / (sigma * sqrt(2))));
}
bool cmp(Backpack &lhs, Backpack &rhs)
{
    return lhs.fitness > rhs.fitness;
}
void print_backpack(Backpack &x)
{
    std::cout << x.current_capacity << " " << x.fitness << " \n";
    for (int i = 0; i < N; i++)
    {
        if (x.inventory[i])
            std::cout << i << " ";
    }
    std::cout << "\n--------------------\n";
}
void print_population(std::vector<Backpack> &population)
{
    for (auto x : population)
    {
        print_backpack(x);
    }
}
void probability_distribution(std::vector<Backpack> &population, std::vector<double> &gene_metric, std::vector<double> &parent_distribution)
{
    // Each parent i has a vector X_i representing his genes,
    // we generate a parenting_weight based on the genes he has,
    // and give a probability based on weight.
    double mean_weight_of_genes = mean(gene_metric);
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        int amount = 0;
        double mean_weight = 0;                                           //
        for (int gene = 0; gene < population[i].inventory.size(); gene++) // maybe change population[i].inventory.size() to N
        {
            if (population[i].inventory[gene]) // if the parent has a certain gene, add its weight
            {
                amount++;
                mean_weight += gene_metric[gene]; // accumulate weights
            }
        }
        mean_weight /= amount; // average them out
        // normalization
        double parent_probability = mean_weight / mean_weight_of_genes;

        // add parent_prob to vector
        parent_distribution[i] = parent_probability;
    }
}
void parent_indexes(std::vector<double> &parent_distribution, int &first, int &second)
{

    std::vector<double> cum_sum_vec(parent_distribution.size());
    std::partial_sum(parent_distribution.begin(), parent_distribution.end(), cum_sum_vec.begin());
    double max_val = *(cum_sum_vec.end() - 1);
    std::uniform_real_distribution<double> unif(0.0, max_val);
    double hit = unif(generator);
    for (int i = 0; i < cum_sum_vec.size(); i++)
    {
        if (cum_sum_vec[i] >= hit)
        {
            first = i;
            break;
        }
    }

    hit = unif(generator);
    for (int i = 0; i < cum_sum_vec.size(); i++)
    {
        if (cum_sum_vec[i] >= hit)
        {
            second = i;
            break;
        }
    }
}
void crossover(const Backpack &parent1, const Backpack &parent2, Backpack &kid1, Backpack &kid2)
{

    std::uniform_int_distribution<> distribution(0, N - 1);
    int breakpoint_1 = distribution(generator);
    std::bernoulli_distribution ber(0.5);

    avg_breakpoint += breakpoint_1;
    crossover_amnt++;

    for (int i = 0; i < N; i++)
    {
        if (ber(generator))
        {
            kid1.inventory[i] = parent1.inventory[i];
            kid2.inventory[i] = parent2.inventory[i];
        }
        else
        {
            kid1.inventory[i] = parent2.inventory[i];
            kid2.inventory[i] = parent1.inventory[i];
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (kid1.inventory[i])
        {
            kid1.fitness += items[i].value;
            kid1.current_capacity += items[i].weight;
        }
        if (kid2.inventory[i])
        {
            kid2.fitness += items[i].value;
            kid2.current_capacity += items[i].weight;
        }
    }

    kid1.fitness = kid1.current_capacity > M ? 0 : kid1.fitness;
    kid2.fitness = kid2.current_capacity > M ? 0 : kid2.fitness;
}
void input()
{
    std::cin >> M >> N;
    items.resize(N);
    for (int i = 0; i < N; i++)
    {
        std::cin >> items[i].weight >> items[i].value;
    }
}
void generate_population(std::vector<Backpack> &population, std::vector<double> &gene_metric)
{
    std::vector<double> cum_sum_vec(N);
    std::partial_sum(gene_metric.begin(), gene_metric.end(), cum_sum_vec.begin());
    double max_val = *(cum_sum_vec.end() - 1);
    std::uniform_real_distribution<double> unif(0.0, max_val);
    double hit = 0;
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        Backpack backpack_i;
        int j = 0;
        while (backpack_i.current_capacity < M && j < 200)
        {
            hit = unif(generator);
            int ind = 0;
            while (cum_sum_vec[ind] < hit)
            {
                ind++;
            }

            if (!backpack_i.inventory[ind] && backpack_i.current_capacity + items[ind].weight < M)
            {
                backpack_i.current_capacity += items[ind].weight;
                backpack_i.fitness += items[ind].value;
                backpack_i.inventory[ind] = true;
            }
            j++;
        }
        if (backpack_i.current_capacity > M)
            backpack_i.fitness = 0;
        population.push_back(backpack_i);
    }
}
void generate_gene_value(std::vector<double> &gene_metric)
{
    double total_value = 0;
    for (int i = 0; i < N; i++)
    {
        double cur_metric = items[i].value * 1.0 / items[i].weight;
        total_value += cur_metric;
        gene_metric[i] = cur_metric;
    }
    // normalization
    for (int i = 0; i < N; i++)
    {
        gene_metric[i] /= total_value;
    }
}
/// We generate the probability to mutate based on a Normal distribution's CDF
/// X~N(mu = 0.5,sigma = 1),i.e mut_prob = P(X < lambda), resulting in a higher chance
/// for a mutation further down the generations;
/// Conversely we want less mutated genes for each mutation later on
/// for this we'll use P(X < lambda) * N , where lambda means how far we are, i.e.
/// lambda = current_generation / MAX_GENERATIONS, and N is the number of genes(items) in
/// the problem.
/// for lambda = 1/2 we get P(X<1/2) = 0.4, and for lambda = 1 we get P(X<1) = 0.6
void mutation(Backpack &individual, std::vector<double> &gene_metric, int current_genertion)
{
    // printf("asdsda1\n");
    double lambda = current_genertion / GENERATIONS;

    double mu = 0.5, sigma = 1;
    double mut_prob = CDF(-lambda, mu, sigma);                      // probability based on a normal distribution X~N(mu,sigma) => P(X>lambda) = mut_prob
    double mutations_count = ceil(CDF(lambda, mu, sigma) * log(N)); //
    // printf("%f\n", CDF(lambda, mu, sigma) );
    std::bernoulli_distribution mutate(mut_prob);

    bool should_mutate = mutate(generator);             // flipping a coin with mut_prob
    std::vector<int> gene_index(N);                     // get a vector of gene indexes to choose from
    std::iota(gene_index.begin(), gene_index.end(), 0); // populate said vector

    global_mutations_success += should_mutate;
    __some_random__index++;

    std::vector<double> cum_sum_vec(N);
    std::partial_sum(gene_metric.begin(), gene_metric.end(), cum_sum_vec.begin());
    double max_val = *(cum_sum_vec.end() - 1);
    std::uniform_real_distribution<double> unif(0.0, max_val);
    double hit = 0;

    if (should_mutate)
    {
        shuffle(gene_index.begin(), gene_index.end(), generator); // shuffle the vector so we get random indexes at the start
        for (int __i = 0; __i < mutations_count; __i++)
        {

            global_mutations_count++;

            hit = unif(generator);
            int ind = 0;
            while (cum_sum_vec[ind] < hit)
            {
                ind++;
            }

            // int ind = gene_index[__i];
            // int ind = gene_index(generator);

            if (individual.inventory[ind])
            {
                individual.current_capacity -= items[ind].weight;
                // individual.fitness -= items[ind].value;
                individual.inventory[ind] = !individual.inventory[ind];
                if (individual.fitness == 0 && individual.current_capacity <= M) // if the backpack was encumbered but is no more, recalculate its fitness
                {
                    for (int i = 0; i < N; i++)
                    {

                        individual.fitness += items[i].value * individual.inventory[i];
                    }
                }
                else if (individual.current_capacity <= M)
                {
                    individual.fitness -= items[ind].value;
                }
            }
            else
            {
                individual.current_capacity += items[ind].weight;
                individual.fitness += items[ind].value;
                individual.inventory[ind] = !individual.inventory[ind];
                if (individual.current_capacity > M)
                {
                    individual.fitness = 0;
                }
            }
        }
    }
}
void genetic_algorithm(std::ofstream &file)
{

    std::vector<double> gene_metric(N); // generate genes cost/value metric
    generate_gene_value(gene_metric);   //

    std::vector<int> display{10, 100, 300, 500, 800};
    std::vector<Backpack> population;
    generate_population(population, gene_metric);

    // int k = 0;
    Backpack best_backpack;
    int cur_gen = 1;

    while (cur_gen <= GENERATIONS)
    {

        std::sort(population.begin(), population.end(), cmp); // sort the last population descending
        std::vector<Backpack> res(0);                         // create space for new population

        // std::vector<Backpack> new_popualtion;                         // new random population to partially
        // generate_population(new_popualtion, gene_metric);             // populate the new generation
        // std::sort(new_popualtion.begin(), new_popualtion.end(), cmp); //
        // new_populations_max_fitness = new_populations_max_fitness < new_popualtion[0].fitness ? new_popualtion[0].fitness : new_populations_max_fitness;

        std::vector<double> parent_distribution(POPULATION_SIZE);               // intialise parent population's
        probability_distribution(population, gene_metric, parent_distribution); // recreating chance distribution

        best_backpack = best_backpack < population[0] ? population[0] : best_backpack; // current best backpack

        if (cur_gen % 5 == 1)
        {
            printf("On generation %d the best backpack is:\n", cur_gen);
            print_backpack(best_backpack);
        }

        /// half the new population is populated by children of
        ///  parents chosen based on their gene weighting
        ///  the other half is the fittest candidates from previous generation and new random generation
        for (int i = 0; i < POPULATION_SIZE / 4; i++)
        {

            Backpack kid1, kid2;

            int first, second;
            parent_indexes(parent_distribution, first, second);
            file << "indexes of parents are " << first << " and " << second << "\n";
            crossover(population[first], population[second], kid1, kid2);
            kids += 2;
            mutation(kid1, gene_metric, cur_gen);
            mutation(kid2, gene_metric, cur_gen);
            kid_best_fitness += kid1.fitness;
            kid_best_fitness += kid2.fitness;
            res.push_back(kid1); // we put the kids in the new generation
            res.push_back(kid2); //

            res.push_back(population[i * 2]);     // we also the fittest individuals from previous
            res.push_back(population[i * 2 + 1]); // generation

            // res.push_back(new_popualtion[i]); // or we add individuals from a new random generation
            // res.push_back(new_popualtion[i*2+1]);        //
        }

        population = res;
        cur_gen++;
    }
    double avg_fitness = 0;
    std::sort(population.begin(), population.end(), cmp);
    for (auto x : population)
    {
        avg_fitness += x.fitness;
        // print_backpack(x);
    }
    printf("On generation %d the best backpack is:\n", cur_gen);
    print_backpack(best_backpack);
    printf("average fitness of final population is %f\n", avg_fitness / POPULATION_SIZE);
}
int main()
{
    std::ofstream myfile;
    myfile.open("pair output.txt", std::ios::trunc);
    input();
    std::cout << "\n--------------------\n";
    genetic_algorithm(myfile);
    myfile.close();

    printf("the average fitness of a kid is %d\n", kid_best_fitness / kids);
    printf("total amount of mutated genes is %d\n", global_mutations_count);
    printf("average mutation chance is %f\n", global_mutations_success / __some_random__index);
    printf("average position for crossover is %f\n", avg_breakpoint / crossover_amnt);

    return 0;
}
//
//
//
//
//
//
//
//
//
//
//
//
//
// NICE