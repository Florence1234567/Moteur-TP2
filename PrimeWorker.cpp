#include "PrimeWorker.h"

bool IsPrime(int n)
{
    if (n < 2)
        return false;

    if (n == 2)
        return true;

    if (n % 2 == 0)
        return false;

    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;

    return true;
}

void PrimerWorkerFunction(PrimeWorker* Worker)
{
    int candidate = 2;

    while (Worker->running.load())
    {
        if (IsPrime(candidate))
        {
            std::lock_guard<std::mutex> lock(Worker->mutex);
            Worker->primes.push_back(candidate);
        }

        candidate++;

        //std::this_thread::yield();
    }
}

void StartWorker(PrimeWorker& Worker)
{
    if (Worker.running.load())
        return;

    std::lock_guard<std::mutex> lock(Worker.mutex);
    Worker.primes.clear();
    Worker.lastReportedIndex = 0;

    Worker.running = true;
    Worker.thread = std::thread(PrimerWorkerFunction, &Worker);
}

void StopWorker(PrimeWorker& Worker)
{
    if (!Worker.running.load())
        return;

    Worker.running = false;

    if (Worker.thread.joinable())
        Worker.thread.join();
}

void PollWorker(PrimeWorker& Worker, std::vector<int>& outNewPrimes)
{
    outNewPrimes.clear();

    std::lock_guard<std::mutex> lock(Worker.mutex);

    const std::size_t total = Worker.primes.size();

    if (Worker.lastReportedIndex >= total)
        return;

    outNewPrimes.assign(
        Worker.primes.begin() + static_cast<std::ptrdiff_t>(Worker.lastReportedIndex),
        Worker.primes.end()
        );

    Worker.lastReportedIndex = total;
}