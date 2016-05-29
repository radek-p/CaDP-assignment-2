//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_LOGGING_H
#define MATRIXMUL_LOGGING_H

#define LOG_LEVEL 3

//enum DBG_SETTINGS
//{
//    DBG_FROM_CSR = 0,
//    DBG_LOADED_SPARSE = 1,
//    DBG_OTHER = 1,
//    DBG_GENERATED_DENSE = 1,
//    DBG_PARTIAL_RESULT = 0,
//    DBG_MULTIPLY = 1,
//    DBG_COLLECT = 0
//};
//
//DBG(MULTIPLY, "p=" << p << " b=" << b << " j=" << j << std::endl);
//#define DBG(type, msg) do\
//	{\
//		if (DBG_SETTINGS::DBG_##type)\
//		{\
//			MPI_Barrier(MPI_COMM_WORLD);\
//			for (int __r = 0; __r < dbg_num_processes; ++__r)\
//			{\
//				if (__r == dbg_mpi_rank)\
//					std::cerr << "[DBG " #type " " << dbg_mpi_rank << " at " << __LINE__ << "]: " << msg;\
//				MPI_Barrier(MPI_COMM_WORLD);\
//			}\
//		}\
//	} while (0)
//
//#define SDBG(type, msg) do\
//	{\
//		if (DBG_SETTINGS::DBG_##type)\
//		{\
//			std::cerr << "[SDBG " #type << " at " << __LINE__ << "]: " << msg;\
//		}\
//	} while (0)
//
//#define bt_assert(x) do\
//	{\
//		if (!(x))\
//		{\
//			void *callstack[128];\
//			int i, frames = backtrace(callstack, 128);\
//			char **strs = backtrace_symbols(callstack, frames);\
//			for (i=0; i<frames; ++i)\
//			{\
//				std::cerr << "[bt] #" << i << ' ' << strs[i] << std::endl;\
//				size_t p = 0;\
//				while(strs[i][p] != '(' && strs[i][p] != ' '\
//				    && strs[i][p] != 0)\
//				++p;\
//				char syscom[256];\
//				sprintf(syscom,"addr2line %p -e %.*s", callstack[i], (int)p, strs[i]);\
//				system(syscom);\
//			}\
//			free(strs);\
//			assert(x);\
//			throw std::runtime_error(#x);\
//		}\
//	} while (0);

#endif //MATRIXMUL_LOGGING_H
