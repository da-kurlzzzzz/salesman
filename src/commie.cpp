#include <iostream>
#include <fstream>
#include <vector>
#include <climits>

/*
 * Эта функция рассчитывает маршрут коммивояжера, начинающего в пункте
 * current_city и заканчивающего в пункте 0 (пункты пронумерованы от 0 до
 * cities_count-1). Для этого перебираются все следующие пункты и для каждого
 * запускается та же функция, рассчитывающая минимальную дорогу от следующего
 * пункта до пункта 0. Если очередной запуск функции обнаружит, что больше
 * никуда идти не надо - он рассчитает дорогу до пункта 0 и завершит работу.
 * */
std::vector<int> takeVoyage(int cities_count,
                            int** country,
                            std::vector<int> not_visited,
                            int current_city,
                            int* way_length) {
    std::vector<int> result(1);
    result[0] = current_city;
    // проверка что вектор пустой, не содержит чисел
    if (not_visited.empty()) {
        *way_length = country[current_city][0];
        return result;
    }
    int min_way_length = INT_MAX;
    // size - количество чисел в векторе
    for (int i = 0; i < (int) not_visited.size(); ++i) {
        std::vector<int> new_not_visited = not_visited;
        // удалить число из вектора
        new_not_visited.erase(new_not_visited.begin() + i);
        std::vector<int> new_result = takeVoyage(cities_count, country, new_not_visited, not_visited[i], way_length);
        // учесть дорогу из текущего города в тот, куда только что отправились
        *way_length += country[current_city][not_visited[i]];
        if (min_way_length > *way_length) {
            min_way_length = *way_length;
            // удалить несколько чисел из вектора
            result.erase(result.begin() + 1, result.end());
            // вставить в конец вектора result все числа из вектора new_result
            result.insert(result.end(), new_result.begin(), new_result.end());
        }
    }
    *way_length = min_way_length;
    return result;
}

std::vector<int> calculateVoyage(int cities_count,
                                 int** country,
                                 int* way_length) {
    std::vector<int> not_visited(cities_count - 1);
    for (int i = 1; i < cities_count; ++i) {
        not_visited[i - 1] = i;
    }
    // запустить коммивояжера в пункте 0
    return takeVoyage(cities_count, country, not_visited, 0, way_length);
}

int main() {
    int len = 7;
    std::ifstream fin("lengths.txt");
    int** country = new int*[len];
    for (int i = 0; i < len; ++i) {
        country[i] = new int[len];
        for (int j = 0; j < len; ++j) {
            fin >> country[i][j];
        }
    }
    fin.close();
    int way_length;
    std::vector<int> result = calculateVoyage(len, country, &way_length);
    std::cout << way_length << "\n";
    for (int i = 0; i < (int) result.size(); ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << "\n";
    return 0;
}
