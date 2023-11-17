/*! \file
 *  \brief GblTimer simple system timer/stopwatch API
 *  \ingroup utils
 *
 *  This file contains GblTimer and its respective API.
 *  GblTimer is a nanosecond-resolution stopwatch-style
 *  API for measuring the amount of time elapsed between
 *  two points in time.
 *
 *  \todo
 *      - make profiler out of a stack of these
 *
 *  \author     2023 Falco Girgis
 *  \copyright  MIT License
 */
#ifndef GIMBAL_TIMER_H
#define GIMBAL_TIMER_H

#include "gimbal_date_time.h"

/*! \name Initialization
 *  \brief Macros used for initializing a timer
 *  @{
 */
#define GBL_TIMER_INIT      { 0, 0, 0 }             //!< Value to assign to a GblTimer during initialization
#define GBL_TIMER(name)     name = GBL_TIMER_INIT   //!< Declares a GblTimer with the given name and initializes it
//! @}

#define GBL_SELF_TYPE GblTimer

GBL_DECLS_BEGIN

/*! Represents a stopwatch-like nanosecond timer
 *  \ingroup utils
 *
 *  GblTimer is a nanosecond-resolution stopwatch-like
 *  timing API which is used for measuring elapsed time
 *  between a start and stop point.
 */
typedef struct GblTimer {
    GblTimeSpec startTime;   //!< Initial starting timestamp
    GblTimeSpec stopTime;    //!< Ending or stopping timestamp
    GblTimeSpec elapsedTime; //!< Calculated elapsed time
    GblBool     active;      //!< Whether the timer is active or not
} GblTimer;

/*! \name        Capturing
 *  \relatesalso GblTimer
 *  \brief       Methods used for capturing timing data
 *  @{
 */
//! Starts the timer, which will continue running until GblTimer_stop() is called
GBL_EXPORT void GblTimer_start    (GBL_SELF) GBL_NOEXCEPT;
//! Stops the timer, recording the amount of time elapsed within GblTimer::elapsedTime
GBL_EXPORT void GblTimer_stop     (GBL_SELF) GBL_NOEXCEPT;
//! Continues the timer, recording elapsed time where it left off
GBL_EXPORT void GblTimer_continue (GBL_SELF) GBL_NOEXCEPT;
//! @}

/*! \name Elapsed Time
 *  \relatesalso GblTimer
 *  \brief       Methods for measuring elapsed time
 *  @{
 */
//! Returns GBL_TRUE if the timer contains a valid elapsed time measurement
GBL_EXPORT GblBool  GblTimer_isValid     (GBL_CSELF) GBL_NOEXCEPT;
//! Returns the amount of time elapsed in seconds
GBL_EXPORT double   GblTimer_elapsedSecs (GBL_CSELF) GBL_NOEXCEPT;
//! Returns the amount of time elapsed in milliseconds
GBL_EXPORT double   GblTimer_elapsedMs   (GBL_CSELF) GBL_NOEXCEPT;
//! Returns the amount of time elapsed in microseconds
GBL_EXPORT double   GblTimer_elapsedUs   (GBL_CSELF) GBL_NOEXCEPT;
//! Returns the amount of time elapsed in nanoseconds
GBL_EXPORT uint64_t GblTimer_elapsedNs   (GBL_CSELF) GBL_NOEXCEPT;
//! @}

GBL_DECLS_END

#undef GBL_SELF_TYPE

#endif // GIMBAL_TIMER_H
