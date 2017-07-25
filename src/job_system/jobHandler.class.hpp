#ifndef JOBHANDLER_CLASS_HPP
# define JOBHANDLER_CLASS_HPP

#include "jobSystem.hpp"

#include <unistd.h>
#include <thread>
#include <mutex>

#define NUMTHREAD 8
#define MAXJOB 65536 * 4

class jobHandler {

public:

	static void init();
	static void shutdown();

	static void 	push_job(t_job job);
	static t_job 	grab_job();
	static bool		slot_avalaible();
	
	static void job_worker(t_job queue[MAXJOB]);

private:
	

	static t_job 				queue[MAXJOB];
	static std::thread			slaves[NUMTHREAD];
	static uint32_t				q_top;
	static std::mutex 			mut;
	static int					must_leave;
};

#endif