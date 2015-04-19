#ifndef PAIRWISE_ERROR_H
#define PAIRWISE_ERROR_H

/*******************************************************************************

    Symbol: PAIRWISE_RETURN_*
    
    Type: Family of preprocessor constants
    
    Intent: Public
    
    Description:
    
        Return codes for libpairwise public functions.

*******************************************************************************/

#define PAIRWISE_RETURN_SUCCESS 0

#define PAIRWISE_RETURN_MALLOC_FAIL 1

#define PAIRWISE_RETURN_PTHREAD_CREATE_EAGAIN 2
#define PAIRWISE_RETURN_PTHREAD_CREATE_EINVAL 3
#define PAIRWISE_RETURN_PTHREAD_CREATE_EPERM 4
#define PAIRWISE_RETURN_PTHREAD_CREATE_UNKNOWN 5

#define PAIRWISE_RETURN_PTHREAD_JOIN_EDEADLK 6
#define PAIRWISE_RETURN_PTHREAD_JOIN_EINVAL 7
#define PAIRWISE_RETURN_PTHREAD_JOIN_ESRCH 8
#define PAIRWISE_RETURN_PTHREAD_JOIN_UNKNOWN 9

#define PAIRWISE_RETURN_ERROR_NCOLLECTIONS 10
#define PAIRWISE_RETURN_ERROR_ICOLLECTIONA 11
#define PAIRWISE_RETURN_ERROR_ICOLLECTIONB 12
#define PAIRWISE_RETURN_ERROR_ICOLLECTIONSAME 13
#define PAIRWISE_RETURN_ERROR_NTHREADS 14

#endif /* PAIRWISE_ERROR_H */
