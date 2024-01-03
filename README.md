Simulation of Construction Site Management System

Objective:
Design and implement a simulated construction site management system using operating
system concepts. This project will focus on managing resources, coordinating tasks among
workers, and handling synchronization in a construction environment.
Scenario:
You are tasked with simulating a construction site where workers need to build a building. The
construction site has limited resources such as bricks, cement, and tools. Workers have specific
tasks like laying bricks, mixing cement, and constructing the building. The bricks and the cement
are transferred to the workers using a wheel cart which acts as a dispatcher. At a given time, only
1 unit of bricks or cement can be transferred to the workers, so the workers have to wait for
sufficient units of bricks and/or cements before they start constructing the building. If the
required units of cement/brick are unavailable, then the abundant resource cannot be produced
until sufficient units of the second resource have been transferred to the workers. Your goal is
to create an operating system-based simulation that efficiently manages these resources and
coordinates tasks among the workers.
Tasks:
You have to do the following tasks for the simulation:
1. Resource Management:
• Simulate the availability of construction resources such as bricks, cement, tools, and
dynamically generated resources.
• Implement resource allocation mechanisms to ensure controlled access by workers.
• Introduce resource replenishment or degradation based on usage, external factors, or
time-based decay.
2. Process and Thread Management:
• Represent each construction task (laying bricks, mixing cement, scaffolding) as
separate threads.
• Simulate the creation and termination of threads based on the availability of tasks and
resources.
• Utilize thread synchronization mechanisms for coordinated access to shared
resources.
3. Synchronization:
• Develop synchronization mechanisms to manage access to shared resources like
bricks, cement, and tools.
• Ensure that simultaneous access to these resources does not lead to conflicts and
implement synchronization to maintain data integrity.
• Employ mutex, semaphores for synchronization.

4. Memory Management:
• Implement memory management to store data related to the construction site,
including resource availability, worker status, task progress, and now, dynamic
adjustments based on task priorities.
• Optimize memory usage for quick data retrieval and updates.
• Utilize shared memory for inter-thread communication.
5. Priority Scheduling:
• Implement a priority scheduling algorithm to efficiently handle critical tasks.
• Define criteria for task prioritization, such as urgent repairs or specific construction
phases.
• Adapt dynamically to changes in task priorities based on real-time conditions.
Step 5.1 Assigning Priorities:
• Each construction task is associated with a priority level. Higher-priority tasks are
assigned lower priority numbers.
• Priority levels are defined based on the criticality and impact of tasks on the overall
project timeline.
Step 5.2 Example Priority Levels:
• High Priority Tasks: Urgent repairs, foundation laying, critical structural work.
• Medium Priority Tasks: General construction tasks, bricklaying, cement mixing.
• Low Priority Tasks: Non-critical tasks, finishing touches, aesthetic elements.
Step 5.3 Assignment of Priority Levels:
• Tasks are categorized into different priority levels during the project planning phase.
• Each task is tagged with its priority level, reflecting its importance to the construction
project.
Step 5.4 Scheduling Logic:
• The priority scheduling algorithm selects the task with the highest priority for
execution first.
• If two tasks have the same priority, other scheduling algorithms or criteria, such as
first-come-first-served, may be employed as tiebreakers.
Step 5.5 Maintain Separate Task Queues:
• The system maintains separate task queues for each priority level (high, medium, low).
• Each queue contains tasks assigned to its respective priority level.
Step 5.6 Selecting Tasks:
• The system selects tasks from the highest priority queue for execution first.
• If the high-priority queue is empty, tasks from the medium priority queue are
considered, and so on.
• The selected tasks are then assigned to available workers or resources for execution.

Step 5.7 Dynamic Priority Adjustments:
• The priority scheduling should be dynamic, allowing the system to adapt to changes in
task priorities based on external factors or project requirements.
• If an urgent repair is identified or if there are changes in project timelines, the priority
of tasks can be dynamically adjusted.
Step 5.8 Integration with I/O Simulation:
• Integrate the priority scheduling algorithm with an I/O simulation that mimics adverse
weather conditions.
• During bad weather, certain tasks (e.g., outdoor construction) may need to be
postponed, and the scheduling algorithm must adjust priorities accordingly.
Step 5.9 Consideration of Worker Skills:
• When assigning tasks, the system considers the skillsets of individual workers.
• Ensure that workers with the required skills are available for high-priority tasks to
optimize efficiency.
Step 5.10 Efficient Resource Utilization:
• Optimize resource utilization by assigning high-priority tasks to the most skilled and
available workers.
Step 5.11 Dynamic Changes in Priority:
• Allow for dynamic adjustments in task priorities based on real-time conditions.
• For instance, if an urgent repair is identified, it should be escalated to a higher priority,
and the scheduling algorithm should adapt accordingly.
6. I/O Management:
• Integrate I/O operations to simulate external factors affecting the construction site,
such as weather conditions.
• Simulate delays in construction due to adverse weather and coordinate worker
activities accordingly.
7. Error Handling:
• Implement robust error-handling mechanisms for unexpected scenarios, such as
resource shortages, task conflicts, adverse weather conditions, and budget overruns.
• Ensure that the system can recover gracefully from errors without compromising
construction site integrity.
8. Dynamic Resource Generation:
• Simulate the dynamic generation of resources over time.
• Implement mechanisms for resource replenishment or degradation based on historical
data and usage patterns.

9. Dynamic Task Assignment:
• Develop algorithms for dynamically assigning tasks to workers based on their skills,
proficiency, and the current construction site needs. - Consider factors like task
dependencies and adjust task assignments accordingly.
10. Realistic Worker Behavior:
• Simulate realistic worker behavior, including fatigue, breaks, and work shifts.
• If a worker wants to get on break (due to any reason), then that worker is swapped
out of the queue as it is useless for the processor now. For the replacement of the
worker, you can use the LRU technique.

#Run Project
1. First open the Ubuntu and save the file on your location.
2. Open the terminal where the file is cloned or saved.
3. Type the command "g++ -o anyname project.cpp"
4. Then type "./anyname" to run the file
