#include "mcv_platform.h"
#include "module_multithread.h"
#include "tbb/tbb.h"

bool CModuleMultithread::start() {
	int ndefThreads = tbb::task_scheduler_init::default_num_threads();
	tbb::task_scheduler_init init(ndefThreads);
	return true;
}

void CModuleMultithread::update(float delta) {
}
