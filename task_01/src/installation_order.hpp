#pragma once

#include <queue>
#include <vector>

inline std::vector<int> installation_order(
    const std::vector<std::vector<int>>& dependents) {
    const int n = static_cast<int>(dependents.size());

    std::vector<int> remaining(n, 0);
    for (const auto& neighbors : dependents) {
        for (int dependent : neighbors) {
            ++remaining[dependent];
        }
    }

    std::queue<int> ready;
    for (int component = 0; component < n; ++component) {
        if (remaining[component] == 0) {
            ready.push(component);
        }
    }

    std::vector<int> order;
    order.reserve(n);

    while (!ready.empty()) {
        const int component = ready.front();
        ready.pop();
        order.push_back(component);

        for (int dependent : dependents[component]) {
            if (--remaining[dependent] == 0) {
                ready.push(dependent);
            }
        }
    }

    return order;
}