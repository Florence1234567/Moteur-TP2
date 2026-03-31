#pragma once

#ifndef CPRIMERWORKER_H
#define CPRIMERWORKER_H

#include <atomic>
#include <cwchar>
#include <mutex>
#include <thread>
#include <vector>

struct PrimeWorker
{
    std::mutex mutex;
    std::thread thread;
    std::vector<int> primes;
    std::size_t lastReportedIndex { 0 };
    std::atomic<bool> running { false };
};

bool IsPrime(int n);

void PrimerWorkerFunction(PrimeWorker* Worker);

void StartWorker(PrimeWorker& Worker);

void StopWorker(PrimeWorker& Worker);

void PollWorker(PrimeWorker& Worker, std::vector<int>& outNewPrimes);

#endif
