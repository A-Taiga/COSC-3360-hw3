#include "huffmanTree.h"
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <string>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>



/// AHHHHHHHHHHHHHHHHH
struct packet
{
	pthread_mutex_t* sem;
	pthread_mutex_t* sem2;
	pthread_cond_t*  con;
	pthread_cond_t*  con2;
	std::string*     line;
	std::string*     msg;
	huffman::Node*   node;
	size_t*          num;
	size_t*          turn;
	bool*            fire;
};

void* handler(void* void_ptr)
{
	std::string   code = {};
	std::string   line = {};
	huffman::Node node;
	size_t        pos = 0;
	size_t        num = 0;
	packet*       p   = (packet*)void_ptr;

	pthread_mutex_lock(p->sem);
	line     = *p->line;
	num      = *p->num;
	node     = (*p->node);
	*p->fire = true;
	pthread_mutex_unlock(p->sem);
	pthread_cond_signal(p->con);

	code = huffman::decode(line, node, *p->msg);

	pthread_mutex_lock(p->sem2);
	while (num != *p->turn)
		pthread_cond_wait(p->con2, p->sem2);

	std::cout << "Symbol: " << node.symbol << ", Frequency: " << node.freq << ", Code: " << code
	          << std::endl;
	(*p->turn)++;
	pthread_mutex_unlock(p->sem2);
	pthread_cond_signal(p->con2);

	return nullptr;
}

int read_file(huffman::priority& pq)
{
	std::string line  = {};
	int         lines = 0;

	std::getline(std::cin, line);
	lines = std::stoi(line);
	for (auto i = 0; i < lines; i++)
	{
		std::getline(std::cin, line);
		char              symbol;
		int               freq;
		std::stringstream ss{line};
		if (line[0] == ' ')
			symbol = ' ';
		else
			ss >> symbol;
		ss >> freq;
		huffman::insert(pq, symbol, freq);
	}

	return lines;
}

int main(int argc, char* argv[])
{
	pthread_mutex_t sem  = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t sem2 = PTHREAD_MUTEX_INITIALIZER;

	pthread_cond_t con  = PTHREAD_COND_INITIALIZER;
	pthread_cond_t con2 = PTHREAD_COND_INITIALIZER;
	static size_t  turn = 0;

	bool fire = false;

	auto              lines = 0;
	std::string       line  = {};
	huffman::priority pq    = {};

	lines       = read_file(pq);
	size_t size = huffman::merge_tree(pq);

	std::vector<pthread_t> threads(lines);
	std::string            msg(size, '\0');
	packet                 p = {};

	for (size_t i = 0; i < lines; i++)
	{
		fire = false;

		std::getline(std::cin, line);
		p.line = &line;
		p.msg  = &msg;
		p.node = pq.top();
		p.sem  = &sem;
		p.sem2 = &sem2;
		p.con  = &con;
		p.con2 = &con2;
		p.fire = &fire;
		p.turn = &turn;
		p.num  = &i;

		pthread_create(&threads[i], nullptr, handler, &p);

		while (fire == false)
			pthread_cond_wait(&con, &sem);
	}

	for (auto& i : threads)
	{
		pthread_join(i, nullptr);
	}

	std::cout << "Original message: " << msg << std::endl;
	pthread_mutex_destroy(&sem);
	pthread_mutex_destroy(&sem2);

	pthread_cond_destroy(&con);
	pthread_cond_destroy(&con2);

	return 0;
}