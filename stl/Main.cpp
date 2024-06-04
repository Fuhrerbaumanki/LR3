#include "Algorithms.h"
#include "Container.h"
#include "Iterator.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>

struct Goods {
  uint32_t m_id;
  std::string m_name;
  std::string m_manufacturer;
  std::string m_warehouse_address;
  double m_weight;
};

int main() {
  using PairType = std::pair<const uint32_t, Goods>;
  using namespace std;

  UnorderedMap<uint32_t, Goods> cont_1{
      {101, {101, "laptop", "Dell", "New York, 5th Avenue, 15", 2.5}},
      {202, {202, "smartphone", "Samsung", "Seoul, Samsung-ro, 67", 0.3}},
      {303, {303, "tablet", "Apple", "Cupertino, Infinite Loop, 1", 0.6}},
      {404, {404, "monitor", "LG", "Seoul, Yeouido-dong, 20", 4.8}},
      {505, {505, "printer", "HP", "Palo Alto, Page Mill Road, 1501", 5.2}},
      {606, {606, "router", "Cisco", "San Jose, Tasman Drive, 500", 0.7}},
      {707, {707, "camera", "Canon", "Tokyo, Shimomaruko, 3", 0.9}},
      {808, {808, "headphones", "Sony", "Tokyo, Konan, 1", 0.2}},
      {909,
       {909, "keyboard", "Logitech", "Lausanne, Innovation Park, 10", 1.1}},
      {1010,
       {1010, "mouse", "Razer", "San Francisco, Howard Street, 201", 0.1}}};
  cout << "Contents of cont_1: " << endl;
  for_each(cont_1.begin(), cont_1.end(), [](const PairType &pair) {
    cout << "|  Id: " << setw(5) << pair.first << "  |  Name: " << setw(10)
         << pair.second.m_name << "  |  Manufacturer: " << setw(10)
         << pair.second.m_manufacturer << "  |  Warehouse address: " << setw(30)
         << pair.second.m_warehouse_address << "  |  Weight: " << setw(5)
         << pair.second.m_weight << "  |" << endl;
  });
  cout << endl;

  UnorderedMap<uint32_t, Goods> cont_2;
  PairType obj_1 = {111, {111, "watch", "Casio", "Tokyo, Hatsudai, 1", 0.1}};
  PairType obj_2 = {
      222, {222, "speaker", "Bose", "Framingham, The Mountain Road, 100", 1.2}};
  cont_2.Insert(obj_1);
  cont_2[obj_2.first] = obj_2.second;
  cout << "Contents of cont_2: " << endl;
  for_each(cont_2.begin(), cont_2.end(), [](const PairType &pair) {
    cout << "|  Id: " << setw(5) << pair.first << "  |  Name: " << setw(10)
         << pair.second.m_name << "  |  Manufacturer: " << setw(10)
         << pair.second.m_manufacturer << "  |  Warehouse address: " << setw(30)
         << pair.second.m_warehouse_address << "  |  Weight: " << setw(5)
         << pair.second.m_weight << "  |" << endl;
  });
  cout << endl;

  auto iter = findIf(cont_1.begin(), cont_1.end(), [](const PairType &pair) {
    return (7 < pair.second.m_weight && pair.second.m_weight < 9);
  });
  cout << "The first element found with a mass from 7 to 9  from the container "
          "cont_1:"
       << endl;
  cout << "|  Id: " << setw(5) << iter->first << "  |  Name: " << setw(10)
       << iter->second.m_name << "  |  Manufacturer: " << setw(10)
       << iter->second.m_manufacturer << "  |  Warehouse address: " << setw(30)
       << iter->second.m_warehouse_address << "  |  Weight: " << setw(5)
       << iter->second.m_weight << "  |" << endl;
  cout << endl;

  iter->second.m_warehouse_address = "San Francisco, Market Street, 1";
  cout << "The contents of the container cont_1, after replacing the address "
          "of the found element with 'San Francisco, Market Street, 1':"
       << endl
       << endl;
  for_each(cont_1.begin(), cont_1.end(), [](const PairType &pair) {
    cout << "|  Id: " << setw(5) << pair.first << "  |  Name: " << setw(10)
         << pair.second.m_name << "  |  Manufacturer: " << setw(10)
         << pair.second.m_manufacturer << "  |  Warehouse address: " << setw(30)
         << pair.second.m_warehouse_address << "  |  Weight: " << setw(5)
         << pair.second.m_weight << "  |" << endl;
  });

  UnorderedMap<uint32_t, Goods> cont_3{
      {101, {101, "laptop", "Dell", "New York, 5th Avenue, 15", 2.5}},
      {202, {202, "smartphone", "Samsung", "Seoul, Samsung-ro, 67", 0.3}},
      {303, {303, "tablet", "Apple", "Cupertino, Infinite Loop, 1", 0.6}},
      {404, {404, "monitor", "LG", "Seoul, Yeouido-dong, 20", 4.8}},
      {505, {505, "printer", "HP", "Palo Alto, Page Mill Road, 1501", 5.2}},
      {606, {606, "router", "Cisco", "San Jose, Tasman Drive, 500", 0.7}},
      {707, {707, "camera", "Canon", "Tokyo, Shimomaruko, 3", 0.9}},
      {808, {808, "headphones", "Sony", "Tokyo, Konan, 1", 0.2}},
      {909,
       {909, "keyboard", "Logitech", "Lausanne, Innovation Park, 10", 1.1}},
      {1010,
       {1010, "mouse", "Razer", "San Francisco, Howard Street, 201", 0.1}}};

  double weight_threshold = 1.0;
  std::cout << "Items with weight greater than " << weight_threshold << ":"
            << std::endl;

  ofstream out_file("Output.txt");

  auto lambda =
      [weight_threshold](const std::pair<const unsigned int, Goods> &pair) {
        return pair.second.m_weight > weight_threshold;
      };

  copyIf(cont_3.begin(), cont_3.end(), out_file, lambda);

  out_file.close();

  return 0;
}
