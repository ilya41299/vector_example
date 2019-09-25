#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <stack>
#include <queue>

template <typename T>
class Graph 
{
	T structure;
	std::string start_vertex;
	std::unordered_map<std::string, std::vector<std::string>> ribs;
	bool type_graph; // - directed(true), undirected(false);
	bool search_type; // - breadth(true), deep(false);
	std::unordered_set<std::string> visited;

public:
	Graph<T>(std::string set_type_graph, std::string set_vertex, std::unordered_map<std::string, std::vector<std::string>>& set_ribs, bool set_search_type)
	{
		if (set_type_graph == "d")type_graph = true;
		else type_graph = false;
		ribs = set_ribs;
		start_vertex = set_vertex;
		search_type = set_search_type;
	}

	void find_way()
	{
		structure.push(start_vertex);
		bool first_endl = false;
		while (!structure.empty())
		{
			std::string temp = take_v(structure);
			structure.pop();
			if (is_visited(temp)) continue;
			visited.insert(temp);
			if (first_endl) 
			{
				std::cout << std::endl;
			}
			std::cout << temp;
			first_endl = true;
			
			std::sort(ribs[temp].begin(), ribs[temp].end());
			if (search_type) 
			{
				for (int i = 0; i < ribs[temp].size(); i++)
				{
					if (!is_visited(ribs[temp][i]))
					{
						structure.push(ribs[temp][i]);
					}
				}
			}
			else 
			{
				for (int i = ribs[temp].size() - 1; i >= 0; i--) 
				{
					if (!is_visited(ribs[temp][i]))
					{
						structure.push(ribs[temp][i]);
					}
				}
			}
			
		}
	}
private:
	bool is_visited(std::string V)
	{
		auto it = visited.find(V);
		if (it!= visited.end()) return true;
		else return false;

	}

	std::string take_v(std::queue<std::string> &queue_struct) 
	{
		return queue_struct.front();
	}

	std::string take_v(std::stack<std::string> &queue_struct)
	{
		return queue_struct.top();
	}

};


int main()
{
	std::string line, type_graph, vertex, search_type;
	bool type_graph_bool;
	std::unordered_map<std::string, std::vector<std::string>> ribs;
	std::getline(std::cin, line);
	std::istringstream line_stream(line);

	line_stream >> type_graph;
	line_stream >> vertex;
	line_stream >> search_type;

	if (type_graph == "u") type_graph_bool = true; // НЕорграф
	else type_graph_bool = false; // орграф


	while (std::getline(std::cin, line)) 
	{
		std::istringstream line_stream(line);
		std::string vertex_1, vertex_2;
		line_stream >> vertex_1;
		line_stream >> vertex_2;
		ribs[vertex_1].push_back(vertex_2);	
		if (type_graph_bool)
		{
			ribs[vertex_2].push_back(vertex_1);
		}
	}

	for (auto& var: ribs)
	{
		std::sort(var.second.begin(), var.second.end());
	}

	if (search_type == "b") 
	{
		Graph<std::queue<std::string>> My_g(type_graph, vertex, ribs, true);
		My_g.find_way();
	}
	else 
	{
		Graph<std::stack<std::string>> My_g(type_graph, vertex, ribs, false);
		My_g.find_way();
	}
	return 0;
}
