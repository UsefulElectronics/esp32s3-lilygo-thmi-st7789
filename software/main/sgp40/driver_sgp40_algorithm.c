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
 * @file      driver_sgp40_algorithm.c
 * @brief     driver sgp40 algorithm source file
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

#include "driver_sgp40_algorithm.h"

/**
 * @brief algorithm params definition
 */
#define DEFAULT_SAMPLING_INTERVAL                                    (1.f)                     /**< default sampling interval */
#define INITIAL_BLACKOUT                                             (45.f)                    /**< initial blackout */
#define INDEX_GAIN                                                   (230.f)                   /**< index gain */
#define SRAW_STD_INITIAL                                             (50.f)                    /**< sraw std initial */
#define SRAW_STD_BONUS_VOC                                           (220.f)                   /**< sraw std bonus voc */
#define SRAW_STD_NOX                                                 (2000.f)                  /**< sraw std nox */
#define TAU_MEAN_HOURS                                               (12.f)                    /**< tau mean hours */
#define TAU_VARIANCE_HOURS                                           (12.f)                    /**< tau variance hours */
#define TAU_INITIAL_MEAN_VOC                                         (20.f)                    /**< tau initial mean voc */
#define TAU_INITIAL_MEAN_NOX                                         (1200.f)                  /**< tau initial mean nox */
#define INIT_DURATION_MEAN_VOC                                       ((3600.f * 0.75f))        /**< init duration mean voc */
#define INIT_DURATION_MEAN_NOX                                       ((3600.f * 4.75f))        /**< init duration mean nox */
#define INIT_TRANSITION_MEAN                                         (0.01f)                   /**< init transition mean */
#define TAU_INITIAL_VARIANCE                                         (2500.f)                  /**< tau initial variance */
#define INIT_DURATION_VARIANCE_VOC                                   ((3600.f * 1.45f))        /**< init duration variance voc */
#define INIT_DURATION_VARIANCE_NOX                                   ((3600.f * 5.70f))        /**< init duration variance nox */
#define INIT_TRANSITION_VARIANCE                                     (0.01f)                   /**< init transition variance */
#define GATING_THRESHOLD_VOC                                         (340.f)                   /**< gating threshold voc */
#define GATING_THRESHOLD_NOX                                         (30.f)                    /**< gating threshold nox */
#define GATING_THRESHOLD_INITIAL                                     (510.f)                   /**< gating threshold initial */
#define GATING_THRESHOLD_TRANSITION                                  (0.09f)                   /**< gating threshold transition */
#define GATING_VOC_MAX_DURATION_MINUTES                              ((60.f * 3.f))            /**< gating voc max duration minutes */
#define GATING_NOX_MAX_DURATION_MINUTES                              ((60.f * 12.f))           /**< gating nox max duration minutes */
#define GATING_MAX_RATIO                                             (0.3f)                    /**< gating max ratio */
#define SIGMOID_L                                                    (500.f)                   /**< sigmoid l */
#define SIGMOID_K_VOC                                                (-0.0065f)                /**< sigmoid k voc */
#define SIGMOID_X0_VOC                                               (213.f)                   /**< sigmoid x0 voc */
#define SIGMOID_K_NOX                                                (-0.0101f)                /**< sigmoid k nox */
#define SIGMOID_X0_NOX                                               (614.f)                   /**< sigmoid x0 nox */
#define VOC_INDEX_OFFSET_DEFAULT                                     (100.f)                   /**< voc index offset default */
#define NOX_INDEX_OFFSET_DEFAULT                                     (1.f)                     /**< nox index offset default */
#define LP_TAU_FAST                                                  (20.0f)                   /**< lp tau fast */
#define LP_TAU_SLOW                                                  (500.0f)                  /**< lp tau slow */
#define LP_ALPHA                                                     (-0.2f)                   /**< lp alpha */
#define VOC_SRAW_MINIMUM                                             (20000)                   /**< voc sraw minimum */
#define NOX_SRAW_MINIMUM                                             (10000)                   /**< nox sraw minimum */
#define PERSISTENCE_UPTIME_GAMMA                                     ((3.f * 3600.f))          /**< persistence uptime gamma */
#define TUNING_INDEX_OFFSET_MIN                                      (1)                       /**< tuning index offset min */
#define TUNING_INDEX_OFFSET_MAX                                      (250)                     /**< tuning index offset max */
#define TUNING_LEARNING_TIME_OFFSET_HOURS_MIN                        (1)                       /**< tuning learning time offset hours min */
#define TUNING_LEARNING_TIME_OFFSET_HOURS_MAX                        (1000)                    /**< tuning learning time offset hours max */
#define TUNING_LEARNING_TIME_GAIN_HOURS_MIN                          (1)                       /**< tuning learning time gain hours min */
#define TUNING_LEARNING_TIME_GAIN_HOURS_MAX                          (1000)                    /**< tuning learning time gain hours max */
#define TUNING_GATING_MAX_DURATION_MINUTES_MIN                       (0)                       /**< tuning gating max duration minutes min */
#define TUNING_GATING_MAX_DURATION_MINUTES_MAX                       (3000)                    /**< tuning gating max duration minutes max */
#define TUNING_STD_INITIAL_MIN                                       (10)                      /**< tuning std initial min */
#define TUNING_STD_INITIAL_MAX                                       (5000)                    /**< tuning std initial max */
#define TUNING_GAIN_FACTOR_MIN                                       (1)                       /**< tuning gain factor min */
#define TUNING_GAIN_FACTOR_MAX                                       (1000)                    /**< tuning gain factor max */
#define MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING                        (64.f)                    /**< mean variance estimator gamma scaling */
#define MEAN_VARIANCE_ESTIMATOR_ADDITIONAL_GAMMA_MEAN_SCALING        (8.f)                     /**< mean variance estimator additional gamma mean scaling */
#define MEAN_VARIANCE_ESTIMATOR_FIX16_MAX                            (32767.f)                 /**< mean variance estimator fix16 max */

/**
 * @brief     mean variance estimator set parameters
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @note      none
 */
static void a_mean_variance_estimator_set_parameters(sgp40_gas_index_algorithm_t *params)
{
    params->m_mean_variance_estimator_initialized = 0;                                                                                  /* init 0 */
    params->m_mean_variance_estimator_mean = 0.f;                                                                                       /* init mean */
    params->m_mean_variance_estimator_sraw_offset = 0.f;                                                                                /* int offset */
    params->m_mean_variance_estimator_std = params->m_sraw_std_initial;                                                                 /* int std */
    params->m_mean_variance_estimator_gamma_mean = (((MEAN_VARIANCE_ESTIMATOR_ADDITIONAL_GAMMA_MEAN_SCALING *
                                                    MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING) *
                                                   (params->m_sampling_interval / 3600.f)) /
                                                   (params->m_tau_mean_hours + (params->m_sampling_interval / 3600.f)));                /* set mean */
    params->m_mean_variance_estimator_gamma_variance = ((MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING *
                                                       (params->m_sampling_interval / 3600.f)) /
                                                       (params->m_tau_variance_hours + (params->m_sampling_interval / 3600.f)));        /* set variance */
    if ((params->m_algorithm_type == SGP40_ALGORITHM_TYPE_NOX))                                                                         /* nox */
    {
        params->m_mean_variance_estimator_gamma_initial_mean = (((MEAN_VARIANCE_ESTIMATOR_ADDITIONAL_GAMMA_MEAN_SCALING *
                                                                MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING) *
                                                                params->m_sampling_interval) /
                                                               (TAU_INITIAL_MEAN_NOX +
                                                                params->m_sampling_interval));                                          /* set mean */
    } 
    else
    {
        params->m_mean_variance_estimator_gamma_initial_mean = (((MEAN_VARIANCE_ESTIMATOR_ADDITIONAL_GAMMA_MEAN_SCALING *
                                                                MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING) *
                                                                params->m_sampling_interval) /
                                                               (TAU_INITIAL_MEAN_VOC +
                                                                params->m_sampling_interval));                                          /* set mean */
    }
    params->m_mean_variance_estimator_gamma_initial_variance = ((MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING *
                                                                params->m_sampling_interval) /
                                                               (TAU_INITIAL_VARIANCE + params->m_sampling_interval));                   /* set variance */
    params->m_mean_variance_estimator_n_gamma_mean = 0.f;                                                                               /* init mean */
    params->m_mean_variance_estimator_n_gamma_variance = 0.f;                                                                           /* init variance */
    params->m_mean_variance_estimator_uptime_gamma = 0.f;                                                                               /* init gamma */
    params->m_mean_variance_estimator_uptime_gating = 0.f;                                                                              /* init gating */
    params->m_mean_variance_estimator_gating_duration_minutes = 0.f;                                                                    /* init minutes */
}

/**
 * @brief     mox model set parameters
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] sraw_std is the source raw std
 * @param[in] sraw_mean is the source raw mean
 * @note      none
 */
static void a_mox_model_set_parameters(sgp40_gas_index_algorithm_t *params, float sraw_std, float sraw_mean)
{
    params->m_mox_model_sraw_std = sraw_std;          /* set std */
    params->m_mox_model_sraw_mean = sraw_mean;        /* set mean */
}

/**
 * @brief     mean variance estimator get std
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @return    mean variance estimator std
 */
static float a_mean_variance_estimator_get_std(const sgp40_gas_index_algorithm_t *params)
{
    return params->m_mean_variance_estimator_std;        /* return std */
}

/**
 * @brief     mean variance estimator get mean
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @return    mean variance estimator get mean
 */
static float a_mean_variance_estimator_get_mean(const sgp40_gas_index_algorithm_t *params)
{
    return (params->m_mean_variance_estimator_mean +
            params->m_mean_variance_estimator_sraw_offset);        /* return mean */
}

/**
 * @brief     sigmoid scaled set parameters
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] x0 is the set x0
 * @param[in] k is the set k
 * @param[in] offset_default is the set offset default
 * @note      none
 */
static void a_sigmoid_scaled_set_parameters(sgp40_gas_index_algorithm_t *params, float x0, float k, float offset_default)
{
    params->m_sigmoid_scaled_k = k;                                  /* set k */
    params->m_sigmoid_scaled_x0 = x0;                                /* set x0 */
    params->m_sigmoid_scaled_offset_default = offset_default;        /* set offset default */
}

/**
 * @brief     adaptive lowpass set parameters
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @note      none
 */
static void a_adaptive_lowpass_set_parameters(sgp40_gas_index_algorithm_t *params)
{
    params->m_adaptive_lowpass_a1 = (params->m_sampling_interval /
                                    (LP_TAU_FAST + params->m_sampling_interval));        /* set a1 */
    params->m_adaptive_lowpass_a2 = (params->m_sampling_interval /
                                    (LP_TAU_SLOW + params->m_sampling_interval));        /* set a2 */
    params->m_adaptive_lowpass_initialized = 0;                                          /* init 0 */
}

/**
 * @brief     init instances
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @note      none
 */
static void a_init_instances(sgp40_gas_index_algorithm_t *params)
{
    a_mean_variance_estimator_set_parameters(params);                              /* set parameters */
    a_mox_model_set_parameters(params, 
                               a_mean_variance_estimator_get_std(params),
                               a_mean_variance_estimator_get_mean(params));        /* set parameters */
    if ((params->m_algorithm_type == SGP40_ALGORITHM_TYPE_NOX))                    /* nox */
    {
        a_sigmoid_scaled_set_parameters(params, SIGMOID_X0_NOX,
                                        SIGMOID_K_NOX,
                                        NOX_INDEX_OFFSET_DEFAULT);                 /* set parameters */
    }
    else                                                                           /* voc */
    {
        a_sigmoid_scaled_set_parameters(params, SIGMOID_X0_VOC,
                                        SIGMOID_K_VOC,
                                        VOC_INDEX_OFFSET_DEFAULT);                 /* set parameters */
    }
    a_adaptive_lowpass_set_parameters(params);                                     /* set parameters */
}

/**
 * @brief     mean variance estimator set states
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] mean is the set mean
 * @param[in] std is the set std
 * @param[in] uptime_gamma is the uptime gamma
 * @note      none
 */
static void a_mean_variance_estimator_set_states(sgp40_gas_index_algorithm_t *params, float mean, 
                                                 float std, float uptime_gamma)
{
    params->m_mean_variance_estimator_mean = mean;                        /* set mean */
    params->m_mean_variance_estimator_std = std;                          /* set std */
    params->m_mean_variance_estimator_uptime_gamma = uptime_gamma;        /* set uptime gamma */
    params->m_mean_variance_estimator_initialized = 1;                    /* init 1 */
}

/**
 * @brief     mean variance estimator is initialized
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @return    mean variance estimator initialized
 */
static uint8_t a_mean_variance_estimator_is_initialized(sgp40_gas_index_algorithm_t *params)
{
    return params->m_mean_variance_estimator_initialized;        /* return initialized */
}

/**
 * @brief     mean variance estimator sigmoid set parameters
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] x0 is the set x0
 * @param[in] k is the set k
 * @note      none
 */
static void a_mean_variance_estimator_sigmoid_set_parameters(sgp40_gas_index_algorithm_t *params, float x0, float k)
{
    params->m_mean_variance_estimator_sigmoid_k = k;          /* set k */
    params->m_mean_variance_estimator_sigmoid_x0 = x0;        /* set x0 */
}

/**
 * @brief     mean variance estimator sigmoid process
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @return    mean variance estimator sigmoid
 */
static float a_mean_variance_estimator_sigmoid_process(sgp40_gas_index_algorithm_t *params, float sample)
{
    float x;

    x = (params->m_mean_variance_estimator_sigmoid_k *
        (sample - params->m_mean_variance_estimator_sigmoid_x0));        /* get sigmoid */
    if ((x < -50.f))                                                     /* check x */
    {
        return 1.f;                                                      /* return 1.0 */
    }
    else if ((x > 50.f))                                                 /* check x */
    {
        return 0.f;                                                      /* return 0 */
    }
    else
    {
        return (1.f / (1.f + expf(x)));                                  /* return sigmoid */
    }
}

/**
 * @brief     mean variance estimator calculate gamma
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @note      none
 */
static void a_mean_variance_estimator_calculate_gamma(sgp40_gas_index_algorithm_t *params)
{
    float uptime_limit;
    float sigmoid_gamma_mean;
    float gamma_mean;
    float gating_threshold_mean;
    float sigmoid_gating_mean;
    float sigmoid_gamma_variance;
    float gamma_variance;
    float gating_threshold_variance;
    float sigmoid_gating_variance;

    uptime_limit = (MEAN_VARIANCE_ESTIMATOR_FIX16_MAX - params->m_sampling_interval);                                                 /* set uptime limit */
    if ((params->m_mean_variance_estimator_uptime_gamma < uptime_limit))                                                              /* set uptime limit */
    {
        params->m_mean_variance_estimator_uptime_gamma = (params->m_mean_variance_estimator_uptime_gamma +
                                                          params->m_sampling_interval);                                               /* set gamma */
    }
    if ((params->m_mean_variance_estimator_uptime_gating < uptime_limit))                                                             /* check gating */
    {
        params->m_mean_variance_estimator_uptime_gating = (params->m_mean_variance_estimator_uptime_gating +
                                                           params->m_sampling_interval);                                              /* set gating */
    }
    a_mean_variance_estimator_sigmoid_set_parameters(params, params->m_init_duration_mean,
                                                     INIT_TRANSITION_MEAN);                                                           /* set parameters */
    sigmoid_gamma_mean = a_mean_variance_estimator_sigmoid_process(params, 
                                                                   params->m_mean_variance_estimator_uptime_gamma);                   /* set mean */
    gamma_mean = (params->m_mean_variance_estimator_gamma_mean +
                 ((params->m_mean_variance_estimator_gamma_initial_mean -
                  params->m_mean_variance_estimator_gamma_mean) *
                  sigmoid_gamma_mean));                                                                                               /* set mean */
    gating_threshold_mean = (params->m_gating_threshold + ((GATING_THRESHOLD_INITIAL -
                             params->m_gating_threshold) *
                             a_mean_variance_estimator_sigmoid_process(params, 
                             params->m_mean_variance_estimator_uptime_gating)));                                                      /* set mean */
    a_mean_variance_estimator_sigmoid_set_parameters(params, gating_threshold_mean,
                                                     GATING_THRESHOLD_TRANSITION);                                                    /* set parameters */
    sigmoid_gating_mean = a_mean_variance_estimator_sigmoid_process(params, params->m_gas_index);                                     /* set mean */
    params->m_mean_variance_estimator_n_gamma_mean = (sigmoid_gating_mean * gamma_mean);                                              /* set mean */
    a_mean_variance_estimator_sigmoid_set_parameters(params, params->m_init_duration_variance,
                                                     INIT_TRANSITION_VARIANCE);                                                       /* set parameters */
    sigmoid_gamma_variance = a_mean_variance_estimator_sigmoid_process(params, 
                                                                       params->m_mean_variance_estimator_uptime_gamma);               /* set variance */
    gamma_variance = (params->m_mean_variance_estimator_gamma_variance +
                     ((params->m_mean_variance_estimator_gamma_initial_variance -
                      params->m_mean_variance_estimator_gamma_variance) *
                     (sigmoid_gamma_variance - sigmoid_gamma_mean)));                                                                 /* set variance */
    gating_threshold_variance = (params->m_gating_threshold + ((GATING_THRESHOLD_INITIAL - params->m_gating_threshold) *
                                 a_mean_variance_estimator_sigmoid_process(params, 
                                                                           params->m_mean_variance_estimator_uptime_gating)));        /* set variance */
    a_mean_variance_estimator_sigmoid_set_parameters(params, gating_threshold_variance,
                                                     GATING_THRESHOLD_TRANSITION);                                                    /* set parameters */
    sigmoid_gating_variance = a_mean_variance_estimator_sigmoid_process(params, params->m_gas_index);                                 /* set process */
    params->m_mean_variance_estimator_n_gamma_variance = (sigmoid_gating_variance * gamma_variance);                                  /* set variance */
    params->m_mean_variance_estimator_gating_duration_minutes = (params->m_mean_variance_estimator_gating_duration_minutes +
                                                                ((params->m_sampling_interval / 60.f) * 
                                                                (((1.f - sigmoid_gating_mean) *
                                                                (1.f + GATING_MAX_RATIO)) - GATING_MAX_RATIO)));                      /* set minutes */
    if ((params->m_mean_variance_estimator_gating_duration_minutes < 0.f))                                                            /* check minutes */
    {
        params->m_mean_variance_estimator_gating_duration_minutes = 0.f;                                                              /* init minutes */
    }
    if ((params->m_mean_variance_estimator_gating_duration_minutes > params->m_gating_max_duration_minutes))                          /* check minutes */
    {
        params->m_mean_variance_estimator_uptime_gating = 0.f;                                                                        /* set uptime gating 0 */
    }
}

/**
 * @brief     mean variance estimator process
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] sraw is the source raw
 * @note      none
 */
static void a_mean_variance_estimator_process(sgp40_gas_index_algorithm_t *params, float sraw)
{
    float delta_sgp;
    float c;
    float additional_scaling;

    if ((params->m_mean_variance_estimator_initialized == 0))                                                       /* no inited */
    {
        params->m_mean_variance_estimator_initialized = 1;                                                          /* set 1 */
        params->m_mean_variance_estimator_sraw_offset = sraw;                                                       /* set raw */
        params->m_mean_variance_estimator_mean = 0.f;                                                               /* init 0 */
    }
    else                                                                                                            /* inited */
    {
        if (((params->m_mean_variance_estimator_mean >= 100.f) ||
           (params->m_mean_variance_estimator_mean <= -100.f)))                                                     /* check mean */
        {
            params->m_mean_variance_estimator_sraw_offset = (params->m_mean_variance_estimator_sraw_offset + 
                                                             params->m_mean_variance_estimator_mean);               /* set offset */
            params->m_mean_variance_estimator_mean = 0.f;                                                           /* init mean */
        }
        sraw = (sraw - params->m_mean_variance_estimator_sraw_offset);                                              /* set sraw */
        a_mean_variance_estimator_calculate_gamma(params);                                                          /* set gamma */
        delta_sgp = ((sraw - params->m_mean_variance_estimator_mean) /
                     MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING);                                                        /* set sgp */
        if ((delta_sgp < 0.f))                                                                                      /* check sgp */
        {
            c = (params->m_mean_variance_estimator_std - delta_sgp);                                                /* get c */
        }
        else
        {
            c = (params->m_mean_variance_estimator_std + delta_sgp);                                                /* get c */
        }
        additional_scaling = 1.f;                                                                                   /* set scaling */
        if ((c > 1440.f))                                                                                           /* check c */
        {
            additional_scaling = ((c / 1440.f) * (c / 1440.f));                                                     /* set scaling */
        }
        params->m_mean_variance_estimator_std =
            (sqrtf((additional_scaling *
                  (MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING -
                   params->m_mean_variance_estimator_n_gamma_variance))) *
             sqrtf(((params->m_mean_variance_estimator_std *
                  (params->m_mean_variance_estimator_std /
                  (MEAN_VARIANCE_ESTIMATOR_GAMMA_SCALING * additional_scaling))) +
                  (((params->m_mean_variance_estimator_n_gamma_variance *
                  delta_sgp) / additional_scaling) * delta_sgp))));                                                 /* set std */
        params->m_mean_variance_estimator_mean = (params->m_mean_variance_estimator_mean +
                                                 ((params->m_mean_variance_estimator_n_gamma_mean * 
                                                  delta_sgp) /
                                                  MEAN_VARIANCE_ESTIMATOR_ADDITIONAL_GAMMA_MEAN_SCALING));          /* set mean */
    }
}

/**
 * @brief     mox model process
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] sraw is the source raw
 * @return    raw
 */
static float a_mox_model_process(sgp40_gas_index_algorithm_t *params, float sraw)
{
    if ((params->m_algorithm_type == SGP40_ALGORITHM_TYPE_NOX))        /* nox */
    {
        return (((sraw - params->m_mox_model_sraw_mean) /
                SRAW_STD_NOX) *
                params->m_index_gain);                                 /* return raw */
    }
    else                                                               /* voc */
    {
        return (((sraw - params->m_mox_model_sraw_mean) /
               (-1.f * (params->m_mox_model_sraw_std +
                SRAW_STD_BONUS_VOC))) * params->m_index_gain);         /* return raw */
    }
}

/**
 * @brief     sigmoid scaled process
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] sample is the set sample
 * @return    sigmoid
 */
static float a_sigmoid_scaled_process(sgp40_gas_index_algorithm_t *params, float sample)
{
    float x;
    float shift;

    x = (params->m_sigmoid_scaled_k * (sample - params->m_sigmoid_scaled_x0));        /* get x */
    if ((x < -50.f))                                                                  /* check x */
    {
        return SIGMOID_L;                                                             /* return sigmoid */
    }
    else if ((x > 50.f))                                                              /* check x */
    {
        return 0.f;                                                                   /* return 0 */
    }
    else
    {
        if ((sample >= 0.f))                                                          /* check sample */
        {
            if ((params->m_sigmoid_scaled_offset_default == 1.f))                     /* check default */
            {
                shift = ((500.f / 499.f) * (1.f - params->m_index_offset));           /* set shift */
            }
            else
            {
                shift = ((SIGMOID_L - (5.f * params->m_index_offset)) / 4.f);         /* set shift */
            }
            
            return (((SIGMOID_L + shift) / (1.f + expf(x))) - shift);                 /* return sigmoid */
        }
        else
        {
            return ((params->m_index_offset /
                    params->m_sigmoid_scaled_offset_default) *
                   (SIGMOID_L / (1.f + expf(x))));                                    /* return sigmoid */
        }
    }
}

/**
 * @brief     adaptive lowpass process
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] sample is the set sample
 * @return    adaptive lowpass
 */
static float a_adaptive_lowpass_process(sgp40_gas_index_algorithm_t *params, float sample)
{
    float abs_delta;
    float f1;
    float tau_a;
    float a3;

    if ((params->m_adaptive_lowpass_initialized == 0))                                       /* init 0 */
    {
        params->m_adaptive_lowpass_x1 = sample;                                              /* set sample */
        params->m_adaptive_lowpass_x2 = sample;                                              /* set sample */
        params->m_adaptive_lowpass_x3 = sample;                                              /* set sample */
        params->m_adaptive_lowpass_initialized = 1;                                          /* set init 1 */
    }
    params->m_adaptive_lowpass_x1 = (((1.f - params->m_adaptive_lowpass_a1) *
                                     params->m_adaptive_lowpass_x1) +
                                    (params->m_adaptive_lowpass_a1 * sample));               /* set adaptive lowpass x1 */
    params->m_adaptive_lowpass_x2 = (((1.f - params->m_adaptive_lowpass_a2) *
                                     params->m_adaptive_lowpass_x2) +
                                    (params->m_adaptive_lowpass_a2 * sample));               /* set adaptive lowpass x2 */
    abs_delta = (params->m_adaptive_lowpass_x1 - params->m_adaptive_lowpass_x2);
    if ((abs_delta < 0.f))                                                                   /* check abs delta */
    {
        abs_delta = (-1.f * abs_delta);                                                      /* get delta */
    }
    f1 = expf((LP_ALPHA * abs_delta));                                                       /* get f1 */
    tau_a = (((LP_TAU_SLOW - LP_TAU_FAST) * f1) + LP_TAU_FAST);                              /* set tau a */
    a3 = (params->m_sampling_interval / (params->m_sampling_interval + tau_a));              /* set a3 */
    params->m_adaptive_lowpass_x3 = (((1.f - a3) * 
                                     params->m_adaptive_lowpass_x3) + (a3 * sample));        /* set adaptive lowpass x3 */
    
    return params->m_adaptive_lowpass_x3;                                                    /* return adaptive lowpass x3 */
}

/**
 * @brief     algorithm reset
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 */
void sgp40_algorithm_reset(sgp40_gas_index_algorithm_t *params)
{
    params->m_uptime = 0.f;          /* init uptime */
    params->m_sraw = 0.f;            /* init sraw */
    params->m_gas_index = 0;         /* int gas index */
    a_init_instances(params);        /* init instances */
}

/**
 * @brief     algorithm init with sampling interval
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] algorithm_type is the algorithm type
 * @param[in] sampling_interval is the sampling interval
 */
void sgp40_algorithm_init_with_sampling_interval(sgp40_gas_index_algorithm_t *params, int32_t algorithm_type, float sampling_interval)
{
    params->m_algorithm_type = algorithm_type;                                          /* set algorithm type */
    params->m_sampling_interval = sampling_interval;                                    /* set sampling interval */
    if ((algorithm_type == SGP40_ALGORITHM_TYPE_NOX))                                   /* nox */
    {
        params->m_index_offset = NOX_INDEX_OFFSET_DEFAULT;                              /* set offset */
        params->m_sraw_minimum = NOX_SRAW_MINIMUM;                                      /* set minimum */
        params->m_gating_max_duration_minutes = GATING_NOX_MAX_DURATION_MINUTES;        /* set minutes */
        params->m_init_duration_mean = INIT_DURATION_MEAN_NOX;                          /* set mean */
        params->m_init_duration_variance = INIT_DURATION_VARIANCE_NOX;                  /* set variance */
        params->m_gating_threshold = GATING_THRESHOLD_NOX;                              /* set threshold */
    }
    else
    {
        params->m_index_offset = VOC_INDEX_OFFSET_DEFAULT;                              /* set offset */
        params->m_sraw_minimum = VOC_SRAW_MINIMUM;                                      /* set minimum */
        params->m_gating_max_duration_minutes = GATING_VOC_MAX_DURATION_MINUTES;        /* set minutes */
        params->m_init_duration_mean = INIT_DURATION_MEAN_VOC;                          /* set mean */
        params->m_init_duration_variance = INIT_DURATION_VARIANCE_VOC;                  /* set variance */
        params->m_gating_threshold = GATING_THRESHOLD_VOC;                              /* set threshold */
    }
    params->m_index_gain = INDEX_GAIN;                                                  /* set gain */
    params->m_tau_mean_hours = TAU_MEAN_HOURS;                                          /* set hours */
    params->m_tau_variance_hours = TAU_VARIANCE_HOURS;                                  /* set hours */
    params->m_sraw_std_initial = SRAW_STD_INITIAL;                                      /* set initial */
    sgp40_algorithm_reset(params);                                                      /* reset */
}

/**
 * @brief     algorithm init
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] algorithm_type is the algorithm type
 */
void sgp40_algorithm_init(sgp40_gas_index_algorithm_t *params, int32_t algorithm_type)
{
    sgp40_algorithm_init_with_sampling_interval(params, algorithm_type, DEFAULT_SAMPLING_INTERVAL);        /* init */
}

/**
 * @brief      algorithm get sampling interval
 * @param[in]  *params points to an sgp40 gas index algorithm handle structure
 * @param[out] *sampling_interval points to a sampling interval buffer
 */
void sgp40_algorithm_get_sampling_interval(const sgp40_gas_index_algorithm_t *params, float *sampling_interval)
{
    *sampling_interval = params->m_sampling_interval;        /* get sampling interval */
}

/**
 * @brief      algorithm get states
 * @param[in]  *params points to an sgp40 gas index algorithm handle structure
 * @param[out] *state0 points to a state0 buffer
 * @param[out] *state1 points to a state1 buffer
 */
void sgp40_algorithm_get_states(const sgp40_gas_index_algorithm_t *params, float* state0, float* state1)
{
    *state0 = a_mean_variance_estimator_get_mean(params);        /* get state0 */
    *state1 = a_mean_variance_estimator_get_std(params);         /* get state1 */
}

/**
 * @brief     algorithm set states
 * @param[in] *params points to an sgp40 gas index algorithm handle structure
 * @param[in] state0 is the set state0
 * @param[in] state1 is the set state1
 */
void sgp40_algorithm_set_states(sgp40_gas_index_algorithm_t *params, float state0, float state1)
{
    a_mean_variance_estimator_set_states(params, state0, state1, PERSISTENCE_UPTIME_GAMMA);        /* set states */
    a_mox_model_set_parameters(params,
                               a_mean_variance_estimator_get_std(params),
                               a_mean_variance_estimator_get_mean(params));                        /* set parameters */
    params->m_sraw = state0;                                                                       /* set state0 */
}

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
                                           int32_t gain_factor)
{
    params->m_index_offset = ((float)(index_offset));                                      /* set offset */
    params->m_tau_mean_hours = ((float)(learning_time_offset_hours));                      /* set hours */
    params->m_tau_variance_hours = ((float)(learning_time_gain_hours));                    /* set hours */
    params->m_gating_max_duration_minutes = ((float)(gating_max_duration_minutes));        /* set minutes */
    params->m_sraw_std_initial = ((float)(std_initial));                                   /* set initial */
    params->m_index_gain = ((float)(gain_factor));                                         /* set gain */
    a_init_instances(params);                                                              /* init instances */
}

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
                                           int32_t *gain_factor)
{

    *index_offset = ((int32_t)(params->m_index_offset));                                      /* get offset */
    *learning_time_offset_hours = ((int32_t)(params->m_tau_mean_hours));                      /* get hours */
    *learning_time_gain_hours = ((int32_t)(params->m_tau_variance_hours));                    /* get hours */
    *gating_max_duration_minutes = ((int32_t)(params->m_gating_max_duration_minutes));        /* get minutes */
    *std_initial = ((int32_t)(params->m_sraw_std_initial));                                   /* get std initial */
    *gain_factor = ((int32_t)(params->m_index_gain));                                         /* get gain factor */
}

/**
 * @brief      algorithm process
 * @param[in]  *params points to an sgp40 gas index algorithm handle structure
 * @param[in]  sraw is the source raw
 * @param[out] *gas_index points to a gas index buffer
 */
void sgp40_algorithm_process(sgp40_gas_index_algorithm_t *params, int32_t sraw, int32_t *gas_index)
{
    if ((params->m_uptime <= INITIAL_BLACKOUT))                                                 /* check time */
    {
        params->m_uptime = (params->m_uptime + params->m_sampling_interval);                    /* set time */
    }
    else
    {
        if (((sraw > 0) && (sraw < 65000)))                                                     /* check sraw */
        {
            if ((sraw < (params->m_sraw_minimum + 1)))                                          /* check sraw */
            {
                sraw = (params->m_sraw_minimum + 1);                                            /* set sraw */
            }
            else if ((sraw > (params->m_sraw_minimum + 32767)))                                 /* check sraw */
            {
                sraw = (params->m_sraw_minimum + 32767);                                        /* set sraw */
            }
            params->m_sraw = ((float)((sraw - params->m_sraw_minimum)));                        /* set sraw */
        }
        if (((params->m_algorithm_type == SGP40_ALGORITHM_TYPE_VOC) 
            || a_mean_variance_estimator_is_initialized(params)))                               /* voc */
        {
            params->m_gas_index = a_mox_model_process(params, params->m_sraw);                  /* set ga index */
            params->m_gas_index = a_sigmoid_scaled_process(params, params->m_gas_index);        /* set ga index */
        }
        else
        {
            params->m_gas_index = params->m_index_offset;                                       /* set gas index */
        }
        params->m_gas_index = a_adaptive_lowpass_process(params, params->m_gas_index);          /* set gas index */
        if ((params->m_gas_index < 0.5f))                                                       /* check gas index */
        {
            params->m_gas_index = 0.5f;                                                         /* set gas index */
        }
        if ((params->m_sraw > 0.f))                                                             /* check sraw */
        {
            a_mean_variance_estimator_process(params, params->m_sraw);                          /* sraw */
            a_mox_model_set_parameters(params,
                                       a_mean_variance_estimator_get_std(params),
                                       a_mean_variance_estimator_get_mean(params));             /* set parameters */
        }
    }
    *gas_index = ((int32_t)((params->m_gas_index + 0.5f)));                                     /* get gas index */
}
