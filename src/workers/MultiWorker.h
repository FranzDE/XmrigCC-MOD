#ifndef __MULTIWORKER_H__
#define __MULTIWORKER_H__


#include "net/Job.h"
#include "net/JobResult.h"
#include "workers/Worker.h"


class Handle;

Worker* createMultiWorker(size_t numHashes, Handle *handle);


#endif /* __SINGLEWORKER_H__ */
