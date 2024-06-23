/**
 * Copyright (c) 2022 - present Sensirion AG All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of Sensirion AG nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file      driver_sgp40_algorithm.h
 * @brief     driver sgp40 algorithm header file
 * @version   1.0.0
 * @author    Sensirion
 * @date      2023-08-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/08/30  <td>1.0      <td>Sensirion   <td>first upload
 * </table>
 */

#ifndef DRIVER_SGP40_ALGORITHM_H
#define DRIVER_SGP40_ALGORITHM_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup sgp40_algorithm sgp40 algorithm function
 * @brief    sgp40 algorithm modules
 * @ingroup  sgp40_driver
 * @{
 */

/**
 * @brief sgp40 algorithm type definition
 */
#define SGP40_ALGORITHM_TYPE_VOC        (0)        /**< voc type */
#define SGP40_ALGORITHM_TYPE_NOX        (1)        /**< nox type */

/**
 * @brief sgp40 gas index algorithm structure definition
 */
typedef struct sgp40_gas_index_algorithm_s
{
    uint8_t m_algorithm_type;                                       /**< algorithm type */
    float m_sampling_interval;                                      /**< sampling interval */
    float m_index_offset;                                           /**< index offset */
    int32_t m_sraw_minimum;                                         /**< sraw minimum */
    float m_gating_max_duration_minutes;                            /**< gating max duration minutes */
    float m_init_duration_mean;                                     /**< init duration mean */
    float m_init_duration_variance;                                 /**< init duration variance */
    float m_gating_threshold;                                       /**< gating threshold */
    float m_index_gain;                                             /**< index gain */
    float m_tau_mean_hours;                                         /**< tau mean hours */
    float m_tau_variance_hours;                                     /**< tau variance hours */
    float m_sraw_std_initial;                                       /**< sraw std initial */
    float m_uptime;                                                 /**< uptime */
    float m_sraw;                                                   /**< sraw */
    float m_gas_index;                                              /**< gas index */
    uint8_t m_mean_variance_estimator_initialized;                  /**< mean variance estimator initialized */
    float m_mean_variance_estimator_mean;                           /**< mean variance estimator mean */
    float m_mean_variance_estimator_sraw_offset;                    /**< mean variance estimator sraw offset */
    float m_mean_variance_estimator_std;                            /**< mean variance estimator std */
    float m_mean_variance_estimator_gamma_mean;                     /**< mean variance estimator gamma mean */
    float m_mean_variance_estimator_gamma_variance;                 /**< mean variance estimator gamma variance */
    float m_mean_variance_estimator_gamma_initial_mean;             /**< mean variance estimator gamma initial mean */
    float m_mean_variance_estimator_gamma_initial_variance;         /**< mean variance estimator gamma initial variance */
    float m_mean_variance_estimator_n_gamma_mean;                   /**< mean variance estimator n gamma mean */
    float m_mean_variance_estimator_n_gamma_variance;               /**< mean variance estimator n gamma variance */
    float m_mean_variance_estimator_uptime_gamma;                   /**< mean variance estimator uptime gamma */
    float m_mean_variance_estimator_uptime_gating;                  /**< mean variance estimator uptime gating */
    float m_mean_variance_estimator_gating_duration_minutes;        /**< mean variance estimator gating duration minutes */
    float m_mean_variance_estimator_sigmoid_k;                      /**< mean variance estimator sigmoid k */
    float m_mean_variance_estimator_sigmoid_x0;                     /**< mean variance estimator sigmoid x0 */
    float m_mox_model_sraw_std;                                     /**< mox model sraw std */
    float m_mox_model_sraw_mean;                                    /**< mox model sraw mean */
    float m_sigmoid_scaled_k;                                       /**< sigmoid scaled k */
    float m_sigmoid_scaled_x0;                                      /**< sigmoid scaled x0 */
    float m_sigmoid_scaled_offset_default;                          /**< sigmoid scaled offset default */
    float m_adaptive_lowpass_a1;                                    /**< adaptive lowpass a1 */
    float m_adaptive_lowpass_a2;                                    /**< adaptive lowpass a2 */
    uint8_t m_adaptive_lowpass_initialized;                         /**< adaptive lowpass initialized */
    float m_adaptive_lowpass_x1;                                    /**< adaptive lowpass x1 */
    float m_adaptive_lowpass_x2;                                    /**< adaptive lowpass x2 */
    float m_adaptive_lowpass_x3;                                    /**< adaptive lowpass x3 */
} sgp40_gas_index_algorithm_t;

/**
 * @brief     algorithm reset
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 */
void sgp40_algorithm_reset(sgp40_gas_index_algorithm_t *params);

/**
 * @brief     algorithm init with sampling interval
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] algorithm_type is the algorithm type
 * @param[in] sampling_interval is the sampling interval
 */
void sgp40_algorithm_init_with_sampling_interval(sgp40_gas_index_algorithm_t *params, int32_t algorithm_type, float sampling_interval);

/**
 * @brief     algorithm init
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] algorithm_type is the algorithm type
 */
void sgp40_algorithm_init(sgp40_gas_index_algorithm_t *params, int32_t algorithm_type);

/**
 * @brief      algorithm get sampling interval
 * @param[in]  *params points to an sgp40 gas index algorithm handle structure
 * @param[out] *sampling_interval points to a sampling interval buffer
 */
void sgp40_algorithm_get_sampling_interval(const sgp40_gas_index_algorithm_t *params, float *sampling_interval);

/**
 * @brief     algorithm set states
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] state0 is the set state0
 * @param[in] state1 is the set state1
 */
void sgp40_algorithm_set_states(sgp40_gas_index_algorithm_t *params, float state0, float state1);

/**
 * @brief      algorithm get states
 * @param[in]  *params points to an sgp40 gas index algorithm handle structure
 * @param[out] *state0 points to a state0 buffer
 * @param[out] *state1 points to a state1 buffer
 */
void sgp40_algorithm_get_states(const sgp40_gas_index_algorithm_t *params, float* state0, float* state1);

/**
 * @brief     algorithm set tuning parameters
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] index_offset is the index offset
 * @param[in] learning_time_offset_hours is the learning time offset hours
 * @param[in] learning_time_gain_hours is the learning time gain hours
 * @param[in] gating_max_duration_minutes is the gating max duration minutes
 * @param[in] std_initial is the std initial
 * @param[in] gain_factor is the gain factor
 */
void sgp40_algorithm_set_tuning_parameters(sgp40_gas_index_algorithm_t *params, int32_t index_offset,
                                           int32_t learning_time_offset_hours, int32_t learning_time_gain_hours,
                                           int32_t gating_max_duration_minutes, int32_t std_initial,
                                           int32_t gain_factor);

/**
 * @brief      algorithm get tuning parameters
 * @param[in]  *params points to an sgp40 gas index algorithm handle structure
 * @param[out] *index_offset points to a index offset buffer
 * @param[out] *learning_time_offset_hours points to a learning time offset hours buffer
 * @param[out] *learning_time_gain_hours points to a learning time gain hours buffer
 * @param[out] *gating_max_duration_minutes points to a gating max duration minutes buffer
 * @param[out] *std_initial points to a std initial buffer
 * @param[out] *gain_factor points to a gain factor buffer
 */
void sgp40_algorithm_get_tuning_parameters(const sgp40_gas_index_algorithm_t *params, int32_t *index_offset,
                                           int32_t *learning_time_offset_hours, int32_t *learning_time_gain_hours,
                                           int32_t *gating_max_duration_minutes, int32_t *std_initial,
                                           int32_t *gain_factor);

/**
 * @brief      algorithm process
 * @param[in]  *params points to an sgp40 gas index algorithm handle structure
 * @param[in]  sraw is the source raw
 * @param[out] *gas_index points to a gas index buffer
 */
void sgp40_algorithm_process(sgp40_gas_index_algorithm_t *params, int32_t sraw, int32_t *gas_index);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif