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

## 4. Need Score Calculation

The algorithm used to calculate need score gives a **priority score from 0 to 100** for emergency requests.
The algorithm consists of four main stages:

1) **Input normalisation** : scaling of numerical inputs to the range (0-1).
2) **Base score calculation** : assigning weighted points to each factor.
3) **Risk condition bonuses** : increasing the score when multiple risk factors occur together.
4) **Exponential transformation** : converting the raw score into a normalized priority score.

The objective is to prioritize requests based on the severity of damage, the scale of the affected population, urgency, and unmet needs.

### Input Normalisation

Before calculating the base score, the damage level and number of families affected are normalized.

#### Families affected

```cpp
float normalised_families_affected =
    (families_affected - 10) / 190.0;
```

This transforms the number of affected families using a range of 10 to 200.
For example,
| Families Affected | Normalised value |
|---:|---:|
| 1 — 10 | 0 |
| 2 — 105 | 0.5 |
| 3 — 200 | 1 |

The normalised value is subsequently squared, allowing larger affected populations to contribute disproportionately more to the score. Families affected measure the scale of humanitarian impact of an emergency. It denotes humanitarian need rather than just severity for one household. A weightage of 20% is given to families affected.


This interpretation is based on the valid input range 10–200.

#### Damage level

```cpp
float normalised_damage_level =
    (damage_level - 1) / 2.0;
```

This transforms a damage rating of 1–3 into a range of 0–1.

| Damage level | Normalized value |
|---:|---:|
| 1 — Low | 0 |
| 2 — Moderate | 0.5 |
| 3 — Severe | 1 |

The normalised damage value is raised to the power of 1.5, increasing the relative contribution of more severe damage. Damage level includes damage to homes, buildings, essential facilities, or elevation level, location exposure in case of natural disaster such as floods. In case of an emergency, physical vulnerability is of significant importance. This is highlighted by giving 25% weightage to damage level. 

### Base Score Calculation

The base score is calculated as follows:

```cpp
float base_score =
    25 * pow(normalised_damage_level, 1.5)
    + 20 * pow(normalised_families_affected, 2)
    + (roads_access ? 0 : 15)
    + (flood_warning ? 10 : 0)
    + (medical_need ? 20 : 0)
    + (previous_aid ? -5 : 10);
```

Each factor contributes differently to reflect its intended importance.

| Factor | Scoring rule | Maximum contribution |
|---|---|---:|
| Damage level | $25 \times \ D^{1.5}$ | 25 points |
| Families affected | $20 \times \ F^2$) | 20 points |
| Road access | 15 if inaccessible; otherwise 0 | 15 points |
| Flood warning | 10 if present; otherwise 0 | 10 points |
| Medical need | 20 if present; otherwise 0 | 20 points |
| Previous aid | +10 if no aid; −5 if aid received | 10 points |

Here, \(D\) represents normalized damage and \(F\) represents normalized families affected.

#### Use of exponents

The exponents introduce nonlinear scoring:

- **Damage exponent of 1.5:** gives greater relative emphasis to severe damage than a simple linear scale.
- **Families exponent of 2:** emphasizes larger affected populations more strongly than smaller ones.

#### Road accessibility

```cpp
(roads_access ? 0 : 15)
```
A community that cannot be reached is particularly difficult to assist. Relatively small disruptions to critical roads can cause disproportionately large loss in healthcare accessibility and delayed relied delivery. So 15 points are added in case of damaged or blocked roads.

#### Flood warning

```cpp
(flood_warning ? 10 : 0)
```

An active flood warning adds 10 points to account for an ongoing or imminent threat since the situation has the potential to deteriorate rapidly.

#### Medical need

```cpp
(medical_need ? 20 : 0)
```

Requests involving medical needs receive 20 additional points because untreated medical conditions can become life-threatening.

#### Previous aid

```cpp
(previous_aid ? -5 : 10)
```

This factor accounts for unmet needs:

- No previous aid: +10 points.
- Previous aid received: −5 points.

The assumption is that requests without previous assistance may have greater unmet needs. Receiving aid does not necessarily mean that a community's needs have been met, so this factor should be interpreted alongside the other indicators.

### Risk Condition Bonuses

In addition to the base score, the algorithm identifies combinations of risk factors.

#### Two condition bonuses

```cpp
int two_conditions_bonus =
    ((medical_need && flood_warning) ? 10 : 0)
    + ((medical_need && !roads_access) ? 10 : 0)
    + ((flood_warning && !previous_aid) ? 5 : 0)
    + ((!roads_access && !previous_aid) ? 5 : 0);
```

| Combined conditions | Bonus |
|---|---:|
| Medical need and flood warning | +10 |
| Medical need and inaccessible roads | +10 |
| Flood warning and no previous aid | +5 |
| Inaccessible roads and no previous aid | +5 |

These bonuses represent situations in which multiple challenges occur simultaneously.
For example, a community with medical needs and inaccessible roads may face both urgent health risks and barriers to receiving treatment.

Each condition is evaluated independently, so multiple bonuses can accumulate with the total maximum bonus of +30.

#### Three condition bonuses

```cpp
int three_conditions_bonus =
    ((damage_level == 3 && medical_need && flood_warning) ? 10 : 0)
    + (((families_affected >= 100) && medical_need
        && !roads_access) ? 10 : 0);
```

Two additional combinations receive 10 points each:

- Severe damage, medical need, and an active flood warning.
- At least 100 affected families, medical need, and inaccessible roads.

These bonuses give additional emphasis to combinations of large-scale impact and urgent needs. These additional combinations can also accumulate giving a maximum bonus of +20.

### Total Raw Score

The raw score combines the base score and both bonus categories:

```cpp
float total_score =
    base_score
    + two_conditions_bonus
    + three_conditions_bonus;
```

### Exponential Transformation

The final score is calculated using a normalized exponential saturation function:

```cpp
need_score = 100 *
    ((1 - exp(-total_score / k))
    / (1 - exp(-max_score / k)));
```

The mathematical expression is:

$$
score = 100 \times \frac{1 - e^{-S/K}}{1 - e^{-S_{\max}/K}}
$$

Where:

- score = final priority score
- S = total raw score
- K = saturation constant.
- $S_{\max}$ = maximum raw score.

For the current configuration:

K=60, $S_{\max}$=150

The saturation constant controls how early the score saturate as the raw score increases.
Max raw score is calculated from the sum of maximum possible raw score and maximum possible bonuses. 

#### Use of exponential saturation

The exponential transformation converts the raw score into a 0-100 score with diminishing returns. When a request has few risk factors, each extra factor raises the score a lot, but once the need is already severe, further factors add progressively less, because a request that is already an emergency doesn’t become more urgent with one more factor.A linear transformation would make the final score directly proportional to the raw score. Exponential saturation instead produces a nonlinear relationship:

- Low raw (0-50) scores produce relatively low priority scores, that increase rapidly for any added factor, due to almost linear nature of equation for **low raw scores**.
- High scores (100-150) approach the maximum value (100) more gradually.

The denominator normalises the result in the range 0-1, while multiplication with 100 gives a value in the range 0-100.

#### Reason for choosing K = 60
With K = 30, the scores are almost flat after 90 raw points, so we can’t tell damage level from medium to high. With K = 120, the gaps barely shrink, so there are no diminishing returns. With K= 60, the first 30 points give 43 of the 100, and each later step still adds a visible gap, down to about 6 at the top.

### Negative score protection

```cpp
need_score = (need_score < 0) ? 0 : need_score;
```

This conditional ensures that negative transformed scores are replaced with zero, so all outputs must remain within the 0–100 range.

### Limitations

The weights, exponents, bonuses, and saturation constant are proposed algorithm parameters that have been chosen based on severity. The numeric values are our own estimates and are not derived from real data.

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
| Market Town        | Low          |               200 | Open         | No            | No           | No                   |
| Bridge Colony      | High         |                40 | Damaged      | Yes           | Yes          | Yes                   |

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
| Ayesha Imran, Lubna Hanif   | Testing, documentation, and GitHub management |

## 11. GitHub Repository:

**Repository link:** https://github.com/iyeshaimran/Supply-Relief-Distribution-System

## 12. AI Usage Declaration:
For need score calculation, we designed the raw score formula ourselves but used AI tools to normalise and transform a raw total. It suggested the equation $score = 100 \times \frac{1 - e^{-S/K}}{1 - e^{-S_{\max}/K}}$ , we learned how the equation works, graphed it and, choose the constants according to the needs of the program.

## 13. Conclusion:

The Relief Supply Distribution System demonstrates how fundamental C++ programming concepts can be applied to a practical resource-allocation problem. It calculates a need score from multiple emergency-related factors, applies decision rules to select a relief action, and updates the available inventory after each successful allocation.

By incorporating resource checks, fallback actions, input validation, and summary counters, the program models a basic approach to managing limited relief supplies. Testing with both sufficient and insufficient resources helps assess whether the implemented rules behave as intended.

