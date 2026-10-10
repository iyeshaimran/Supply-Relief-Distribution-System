# Supply-Relief-Distribution-System
## 1. Introduction:
The **Emergency Relief Supply Distribution System** is a menu-driven C++ program designed to simulate the distribution of relief supplies to areas affected by natural disasters. The system collects information about an affected area, calculates its need score between 0 and 100, and selects an appropriate relief action. 

Since trucks and supply packs are limited, the program checks the available resources before allocating assistance. The system also updates the remaining inventory after every request and displays a summary of requests handled and deferred.

## 2. Objectives:

The main objectives of the project are to:

1) Calculate a need score using multiple emergency-related factors.
2) Apply conditional statements to determine the appropriate relief action.
3) Prioritize emergencies while considering available resources.
4) Validate user input to reduce invalid entries.
5) Update inventory accurately after every successful allocation.
6) Display request details, resource usage, and remaining inventory.
7) Summarize the number of requests received, handled, and deferred.

## 3. Input Parameters:
The program collects the following information for each request:

| Input | Possible Values / Range |
|---|---|
| Damage Level | High, Medium, Low |
| Families Affected | 10–200 |
| Access Route | Open, Damaged |
| Flood Warning | Yes, No |
| Medical Need | Yes, No |
| Previous Aid Received | Yes, No |

Damage levels are internally represented as 3 for High, 2 for Medium, and 1 for Low. The yes/no inputs are stored as Boolean values. The program validates the damage-level choice, the number of affected families, and the yes/no responses before proceeding with the calculation.

## 5. Decision and Action Rules:
The system uses the calculated need score to determine which action should be prioritized.

### Need Score and Preferred Action

| Need Score | Preferred Action | Trucks | Supply Packs |
|---|---|---:|---:|
| 75 < score <= 100 | Full Relief | 3 | 60 |
| 50 < score <= 75 | Standard Relief | 2 | 50 |
| 25 < score <= 50 | Emergency Kit Only | 1 | 30 |
| score <= 25 | Defer / Monitor | 0 | 0 |

### Resource-based fallback rules

The program checks whether the required trucks and supply packs are available before allocating assistance.

1. **Full Relief:** If three trucks and 60 supply packs are available, full relief is provided. Otherwise, the program checks whether Standard Relief can be provided. If that is unavailable, it checks whether an Emergency Kit can be provided.
2. **Standard Relief:** If two trucks and 50 supply packs are available, standard relief is provided. Otherwise, the program checks whether an Emergency Kit can be provided.
3. **Emergency Kit Only:** One truck and 30 supply packs must be available.
4. **Defer / Monitor:** Requests with a need score of 25 or below are deferred without allocating resources.

If no suitable allocation can be made, the request is deferred and the program displays a reason, such as insufficient trucks or insufficient supply packs.

### Why these rules were selected

The score thresholds help prioritize requests according to their assessed need. The fallback mechanism allows the system to provide a smaller form of assistance when the preferred action cannot be supported by the remaining inventory.

This approach aims to balance emergency prioritization with responsible resource management. The program checks both trucks and supply packs so that an allocation is made only when the resources required for that action are available.

## 6. Initial Resources and Inventory Management:

The program starts with the following inventory:

| Resource     | Initial quantity |
| ------------ | ---------------: |
| Trucks       |               10 |
| Supply packs |              200 |

For every request that receives assistance, the program subtracts the allocated trucks and supply packs from the available inventory.

Requests that are deferred do not consume resources. This allows subsequent requests to be evaluated using the updated inventory. The system therefore avoids allocating more resources than are available for an individual action.

## 8. Testing and Results

The program was tested using the five requests provided in the assignment. The tests evaluate need score calculation, relief allocation, fallback actions, sequential resource updates, and the handling of requests when resources become insufficient.

### 8.1 Input Cases Provided by the Instructor

The following table summarizes the information provided for the five relief requests.

| Request            | Damage Level | Families Affected | Access Route | Flood Warning | Medical Need | Previous Aid Received |
| ------------------ | ------------ | ----------------: | ------------ | ------------- | ------------ | --------------------- |
| Riverside Village  | High         |               150 | Damaged      | Yes           | Yes          | Yes                   |
| Hilltop Settlement | Medium       |                60 | Open         | No            | No           | No                    |
| Lakeside Camp      | High         |                90 | Open         | Yes           | Yes          | Yes                   |
| Market Town        | Low          |               200 | Open         | No            | No           | No*                   |
| Bridge Colony      | High         |                40 | Damaged      | Yes           | Yes          | Yes                   |

*Note: The Previous Aid Received value for Market Town is assumed to be No. Confirm this value against the original assignment if necessary.*

### 8.2 Test Set 1: Sufficient Initial Resources

**Initial Resources:** 10 Trucks and 200 Supply Packs, as initialized in the code.

| Request            | Need Score | Selected Action    | Trucks Used | Supply Packs Used | Trucks Remaining | Supply Packs Remaining |
| ------------------ | ---------: | ------------------ | ----------: | ----------------: | ---------------: | ---------------------: |
| Riverside Village  |     93.145 | Full Relief        |           3 |                60 |                7 |                    140 |
| Hilltop Settlement |     31.173 | Emergency Kit Only |           1 |                30 |                6 |                    110 |
| Lakeside Camp      |     76.964 | Full Relief        |           3 |                60 |                3 |                     50 |
| Market Town        |     42.866 | Emergency Kit Only |           1 |                30 |                2 |                     20 |
| Bridge Colony      |     86.763 | Defer / Monitor    |           0 |                 0 |                2 |                     20 |

**Summary:**

* Total requests processed: 5
* Requests successfully handled: 4
* Requests deferred: 1
* Final trucks remaining: 2
* Final supply packs remaining: 20

**Observation:** The first four requests were handled according to their need scores. Bridge Colony qualified for Full Relief based on its score, but only 20 supply packs remained. Since the remaining inventory was insufficient for the available relief actions, the request was deferred.

### 8.3 Test Set 2: Continued Testing with Remaining Resources

**Initial Resources:** 2 Trucks and 20 Supply Packs, carried forward from Test Set 1.

This test continues with the inventory remaining after the first test. It demonstrates how the system handles additional requests when resources are limited.

| Request            | Need Score | Selected Action | Trucks Used | Supply Packs Used | Trucks Remaining | Supply Packs Remaining |
| ------------------ | ---------: | --------------- | ----------: | ----------------: | ---------------: | ---------------------: |
| Riverside Village  |     93.145 | Defer / Monitor |           0 |                 0 |                2 |                     20 |
| Hilltop Settlement |     31.173 | Defer / Monitor |           0 |                 0 |                2 |                     20 |
| Lakeside Camp      |     76.964 | Defer / Monitor |           0 |                 0 |                2 |                     20 |
| Market Town        |     42.866 | Defer / Monitor |           0 |                 0 |                2 |                     20 |
| Bridge Colony      |     86.763 | Defer / Monitor |           0 |                 0 |                2 |                     20 |

**Summary:**

* Total requests processed: 5
* Requests successfully handled: 0
* Requests deferred: 5
* Final trucks remaining: 2
* Final supply packs remaining: 20

**Observation:** The remaining inventory was insufficient for the smallest relief action, which requires 1 truck and 30 supply packs. Therefore, all requests were deferred, and the available resources remained unchanged.

### 8.4 Overall Conclusion

The test results demonstrate that the system calculates need scores, selects relief actions according to score thresholds, updates resources sequentially, and defers requests when the available inventory cannot support even the smallest relief action. The summary counters track the number of requests processed, successfully handled, and deferred.


## 9. Summary and Counters:

The program maintains three counters:

* **Requests Received:** Incremented whenever a new emergency request is submitted.
* **Requests Handled:** Incremented when at least one truck is allocated to a request.
* **Requests Deferred:** Incremented when no resources are allocated to a request.

When the user selects the exit option, the program displays a final summary containing all three counts.

## 10. Group Members and Contributions:

| Group member | Contribution                                    |
| ------------ | ----------------------------------------------- |
| Ayesha Imran   | Need-score formula and calculation          |
| Lubna Hanif  | Decision rules and resource allocation        |
| [Ayesha Imran, Lubna Hanif   | Testing, documentation, and GitHub management |

## 11. GitHub Repository:

**Repository link:** 

## 12. AI Usage Declaration:

## 13. Conclusion:

The Relief Supply Distribution System demonstrates how fundamental C++ programming concepts can be applied to a practical resource-allocation problem. It calculates a need score from multiple emergency-related factors, applies decision rules to select a relief action, and updates the available inventory after each successful allocation.

By incorporating resource checks, fallback actions, input validation, and summary counters, the program models a basic approach to managing limited relief supplies. Testing with both sufficient and insufficient resources helps assess whether the implemented rules behave as intended.

