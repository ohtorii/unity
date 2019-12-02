#pragma once
#include<concrt.h>
#include<vector>


template<typename _TYPE>
class ParallelVector : public std::vector<typename _TYPE>, public concurrency::critical_section {
public:
	typedef concurrency::critical_section::scoped_lock scoped_lock;

private:
		
};
