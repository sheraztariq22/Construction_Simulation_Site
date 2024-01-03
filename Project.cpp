#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <queue>
#include <functional>
#include <atomic>
#include <cstdlib>
#include <ctime>

using namespace std;

enum Priority { HIGH, MEDIUM, LOW };

struct Resource {
    int quantity;
    pthread_mutex_t lock;
    sem_t semaphore;
};

struct Task {
    std::function<void()> function;
    atomic<Priority> priority;
};

const int NUM_WORKERS = 3;

struct Worker {
    pthread_t thread;
    bool onBreak;
    time_t lastActiveTime;
};

Worker workers[NUM_WORKERS];

queue<Task*> highPriorityTasks;
queue<Task*> mediumPriorityTasks;
queue<Task*> lowPriorityTasks;
pthread_mutex_t taskQueueMutex;

Resource bricks, cement;
atomic<bool> resourcesDepleted(false);
atomic<bool> badWeather(false);
atomic<bool> skilledWorkerAvailable(true);
atomic<int> budget(1000); // Example budget

void simulateWeather() {
    srand(time(nullptr));
    int weatherRand = rand() % 10;
    if (weatherRand > 5) {
        badWeather = true;
        cout << "[Weather] The weather has turned bad!" << endl;
    } else {
        badWeather = false;
        cout << "[Weather] The weather is good." << endl;
    }
}

void updateWorkerSkills() {
    skilledWorkerAvailable = !skilledWorkerAvailable.load();
    if (skilledWorkerAvailable.load()) {
        cout << "[Skill Update] Skilled worker is available." << endl;
    } else {
        cout << "[Skill Update] No skilled worker available." << endl;
    }
}

// Function to simulate a worker going on break
bool simulateWorkerBreak() {
    return (rand() % 10) < 3; // 30% chance of going on break
}

// Update worker's last active time
void updateWorkerLastActiveTime(Worker& worker) {
    time(&worker.lastActiveTime);
}

Worker* getLRUWorker() {
    Worker* lruWorker = nullptr;
    time_t oldestTime = time(nullptr);

    for (int i = 0; i < NUM_WORKERS; ++i) {
        if (!workers[i].onBreak && workers[i].lastActiveTime < oldestTime) {
            lruWorker = &workers[i];
            oldestTime = workers[i].lastActiveTime;
        }
    }

    return lruWorker;
}

void initializeResource(Resource& resource, int quantity) {
    resource.quantity = quantity;
    pthread_mutex_init(&resource.lock, NULL);
    sem_init(&resource.semaphore, 0, 1);
}

bool checkBudget(int cost) {
    if (budget.load() < cost) {
        if (budget.load() < 30) {
            cout << "[Budget] Budget is low. Requesting additional funds..." << endl;
            budget += 400;
            cout << "[Budget] Received additional 400 from the boss. New Budget: " << budget.load() << endl;
        }
        return false;
    }
    return true;
}

void deductBudget(int cost) {
    budget -= cost;
    cout << "[Budget] Deducted " << cost << " from budget. Remaining Budget: " << budget.load() << endl;
}

void useResource(Resource& resource, int amount, int minThreshold, int maxReplenishAmount, int minReplenishAmount) {
    sem_wait(&resource.semaphore);
    pthread_mutex_lock(&resource.lock);
    if (resource.quantity >= amount) {
        resource.quantity -= amount;
        cout << "[Resource] Used: " << amount << ". Remaining: " << resource.quantity << endl;
    } else {
        cout << "[Resource] Not enough resources. Auto-replenishing." << endl;
        int replenishAmount = rand() % (maxReplenishAmount - minReplenishAmount + 1) + minReplenishAmount;
        resource.quantity += replenishAmount;
        cout << "[Resource] Auto-replenished with " << replenishAmount << ". Total: " << resource.quantity << endl;
    }
    if (resource.quantity < minThreshold) {
        int replenishAmount = rand() % (maxReplenishAmount - minReplenishAmount + 1) + minReplenishAmount;
        resource.quantity += replenishAmount;
        cout << "[Resource] Auto-replenished with " << replenishAmount << ". Total: " << resource.quantity << endl;
    }
    pthread_mutex_unlock(&resource.lock);
    sem_post(&resource.semaphore);
}

void replenishResource(Resource& resource, int minAmount, int maxAmount) {
    int amount = rand() % (maxAmount - minAmount + 1) + minAmount;
    pthread_mutex_lock(&resource.lock);
    resource.quantity += amount;
    cout << "[Resource] Replenished " << amount << ". Total: " << resource.quantity << endl;
    pthread_mutex_unlock(&resource.lock);
}


bool checkResourceAvailability(Resource& resource, int requiredAmount) {
    pthread_mutex_lock(&resource.lock);
    bool isAvailable = resource.quantity >= requiredAmount;
    pthread_mutex_unlock(&resource.lock);
    return isAvailable;
}


void* resourceAddThread(void* arg) {
    while (true) {
        sleep(5);
        replenishResource(bricks, 5, 15);
        replenishResource(cement, 5, 15);
    }
    return NULL;
}

void* layingBricks(void* arg) {
    while (bricks.quantity > 0 && !resourcesDepleted) {
        if (!checkResourceAvailability(bricks, 10) || !checkBudget(50)) {
            cerr << "[Error] Resource shortage or budget overrun in laying bricks." << endl;
            sleep(5);
            continue;
        }
        deductBudget(50); // Example cost deduction
        useResource(bricks, 10, 15, 70, 30);
        cout << "[Action] Worker is laying bricks." << endl;
        sleep(1);
    }
    return NULL;
}
void* mixingCement(void* arg) {
    while (cement.quantity > 0 && !resourcesDepleted) {
        if (!checkResourceAvailability(cement, 10) || !checkBudget(30)) {
            cerr << "[Error] Resource shortage or budget overrun in mixing cement." << endl;
            sleep(5);
            continue;
        }
        deductBudget(30); // Example cost deduction
        useResource(cement, 10, 15, 70, 30);
        cout << "[Action] Worker is mixing cement." << endl;
        sleep(2);
    }
    return NULL;
}

void* scaffolding(void* arg) {
    while (!resourcesDepleted) {
        if ((!checkResourceAvailability(bricks, 5) || !checkResourceAvailability(cement, 5)) || !checkBudget(40)) {
            cerr << "[Error] Resource shortage or budget overrun in scaffolding." << endl;
            sleep(5);
            continue;
        }
        deductBudget(40); // Example cost deduction
        useResource(bricks, 5, 15, 70, 30);
        useResource(cement, 5, 15, 70, 30);
        cout << "[Action] Worker is working on scaffolding." << endl;
        sleep(3);
    }
    return NULL;
}

void* urgentRepairs(void* arg) {
    cout << "[Urgent Task] Performing urgent repairs." << endl;
    sleep(1);
    return NULL;
}

void* foundationLaying(void* arg) {
    cout << "[Important Task] Laying foundation." << endl;
    sleep(2);
    return NULL;
}

void* criticalStructuralWork(void* arg) {
    cout << "[Critical Task] Doing critical structural work." << endl;
    sleep(3);
    return NULL;
}

void* generalConstructionTasks(void* arg) {
    cout << "[General Task] Performing general construction tasks." << endl;
    sleep(2);
    return NULL;
}

void* finishingTouches(void* arg) {
    cout << "[Finishing Task] Applying finishing touches." << endl;
    sleep(1);
    return NULL;
}

void* aestheticElements(void* arg) {
    cout << "[Aesthetic Task] Working on aesthetic elements." << endl;
    sleep(1);
    return NULL;
}

void addTask(Task* task) {
    pthread_mutex_lock(&taskQueueMutex);
    if (!resourcesDepleted) {
        switch (task->priority) {
            case HIGH:
                highPriorityTasks.push(task);
                cout << "[Task Queue] High priority task added." << endl;
                break;
            case MEDIUM:
                mediumPriorityTasks.push(task);
                cout << "[Task Queue] Medium priority task added." << endl;
                break;
            case LOW:
                lowPriorityTasks.push(task);
                cout << "[Task Queue] Low priority task added." << endl;
                break;
          }
    }
    pthread_mutex_unlock(&taskQueueMutex);
}

Task* getNextTask() {
    Task* task = nullptr;
    pthread_mutex_lock(&taskQueueMutex);
    if (!highPriorityTasks.empty()) {
        task = highPriorityTasks.front();
        highPriorityTasks.pop();
    } else if (!mediumPriorityTasks.empty()) {
        task = mediumPriorityTasks.front();
        mediumPriorityTasks.pop();
    } else if (!lowPriorityTasks.empty()) {
        task = lowPriorityTasks.front();
        lowPriorityTasks.pop();
    }
    pthread_mutex_unlock(&taskQueueMutex);
    return task;
}

void executeTask(Task* task) {
    if (task) {
        task->function();
        delete task;
    }
}

void* workerThread(void* arg) {
    Worker* worker = static_cast<Worker*>(arg);

    while (!resourcesDepleted) {
        simulateWeather();
        updateWorkerSkills();

        if (badWeather.load()) {
            cerr << "[Error] Bad weather, pausing tasks." << endl;
            sleep(5);
            continue;
        }

        if (!skilledWorkerAvailable.load()) {
            cerr << "[Error] No skilled workers available, pausing tasks." << endl;
            sleep(5);
            continue;
        }

        if (budget.load() <= 0) {
            cerr << "[Error] Budget depleted." << endl;
            sleep(5);
            continue;
        }

        if (simulateWorkerBreak()) {
            worker->onBreak = true;
            cout << "[Worker] Worker is taking a break." << endl;
            sleep(10); // Simulate break time
            worker->onBreak = false;
            updateWorkerLastActiveTime(*worker);
            cout << "[Worker] Worker is back from break." << endl;
            continue;
        }

        Task* task = getNextTask();
        if (task) {
            cout << "[Worker] Worker is starting a new task." << endl;
            executeTask(task);
            cout << "[Worker] Worker has completed a task." << endl;
        } else {
            sleep(1); // Sleep to give other threads a chance to execute
        }
    }
    return NULL;
}

int main() {
    srand(time(nullptr));

    // Initialize mutex
    if (pthread_mutex_init(&taskQueueMutex, NULL) != 0) {
        cerr << "Failed to initialize mutex. Exiting program." << endl;
        return 1;
    }

    // Initialize resources
    initializeResource(bricks, 100);
    initializeResource(cement, 50);

    // Set initial conditions
    badWeather = false;
    skilledWorkerAvailable = true;

    cout << "[Initialization] Resources and conditions are set." << endl;

    // Create and add tasks
    Task* tasks[] = {new Task(), new Task(), new Task(), new Task(), new Task(), new Task(), new Task(), new Task()};
    tasks[0]->function = []{ urgentRepairs(nullptr); };
    tasks[0]->priority = HIGH;
    tasks[1]->function = []{ foundationLaying(nullptr); };
    tasks[1]->priority = HIGH;
    tasks[2]->function = []{ criticalStructuralWork(nullptr); };
    tasks[2]->priority = HIGH;
    tasks[3]->function = []{ generalConstructionTasks(nullptr); };
    tasks[3]->priority = MEDIUM;
    tasks[4]->function = []{ layingBricks(nullptr); };
    tasks[4]->priority = MEDIUM;
    tasks[5]->function = []{ mixingCement(nullptr); };
    tasks[5]->priority = MEDIUM;
    tasks[6]->function = []{ finishingTouches(nullptr); };
    tasks[6]->priority = LOW;
    tasks[7]->function = []{ aestheticElements(nullptr); };
    tasks[7]->priority = LOW;

    for (Task* task : tasks) {
        addTask(task);
    }

    cout << "[Initialization] Tasks are created and added to the queue." << endl;

    // Initialize and create worker threads
    for (int i = 0; i < NUM_WORKERS; ++i) {
        workers[i].onBreak = false;
        time(&workers[i].lastActiveTime);

        if (pthread_create(&workers[i].thread, NULL, workerThread, &workers[i]) != 0) {
            cerr << "Failed to create worker thread. Exiting program." << endl;
            return 1;
        }
    }

    cout << "[Initialization] Worker threads are created and running." << endl;

    // Create resource adder thread
    pthread_t resourceAdder;
    if (pthread_create(&resourceAdder, NULL, resourceAddThread, NULL) != 0) {
        cerr << "Failed to create resource adder thread. Exiting program." << endl;
        return 1;
    }
    pthread_detach(resourceAdder);

    cout << "[Initialization] Resource adder thread is running." << endl;

    // Wait for worker threads to complete
    for (int i = 0; i < NUM_WORKERS; ++i) {
        if (pthread_join(workers[i].thread, NULL) != 0) {
            cerr << "Failed to join worker thread." << endl;
        }
    }

    // Cleanup
    pthread_mutex_destroy(&taskQueueMutex);
    pthread_mutex_destroy(&bricks.lock);
    pthread_mutex_destroy(&cement.lock);

    cout << "Program completed successfully." << endl;

    return 0;
}