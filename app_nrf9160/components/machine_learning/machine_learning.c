#include "machine_learning.h"

static size_t frame_surplus;
static void result_ready_cb(int err);

float sample_data1[156] =
    {
        0.3100, -0.6900, -0.9200, 1.2300, -0.9600, -1.3400, 0.5400, -4.3700, -2.8300, 0.3800, -3.7900, -2.6800, 1.0000, -2.6800, -2.4500, 1.8000,
        -3.1000, -3.1400, 1.3000, -2.7600, -3.9800, 0.4600, -1.6900, -3.9500, 1.2300, -0.8400, -2.8300, 1.6100, -1.2300, -3.1400, 1.0000, -2.1500, -4.2100, 0.1500, -1.2600,
        -3.9800, -0.3100, 0.6900, -3.2200, -0.1100, 0.7300, -3.4900, -1.1500, -0.3100, -5.5200, -2.5300, 0.6900, -7.9300, -1.9200, 3.2200, -5.5900, -1.6900, 4.2500, -5.7800,
        -1.0000, 2.7600, -8.2700, -1.4900, 1.9200, -9.1900, -3.9800, 3.3700, -9.4200, -4.7900, 4.9000, -10.5700, -4.5200, 5.7500, -14.5600, -3.2600, 5.1700, -16.0900, -1.9900,
        3.2200, -19.0000, -3.7500, 2.0300, -19.9600, -7.5800, 0.8400, -20.3800, -7.8100, 1.0000, -20.2300, -6.0500, 1.2300, -19.7700, -4.2100, 0.8400, -19.6100, -2.7600,
        1.3800, -19.0800, -2.3800, 1.6500, -18.2700, -2.1500, 2.4500, -17.0800, -2.4900, 2.9900, -17.0100, -3.4500, 3.7500, -16.4000, -3.7500, 4.0600, -16.2800, -4.0600,
        4.1400, -14.9400, -3.8300, 3.9500, -13.8300, -2.8300, 3.9800, -11.3400, -2.3800, 3.6000, -10.2700, -2.4500, 3.5200, -9.1900, -2.2600, 3.7500, -8.8500, -0.6900,
        4.0600, -7.6600, 0.3400, 3.8700, -7.1300, -0.1500, 1.5300, -5.5200, -0.0400, 0.6900, -4.6400, 0.4600, -0.9200, -1.0700, 0.7700, -1.0700, -0.4600, 2.5300, -2.2200,
        -1.7600, 2.9100, -1.9500, -2.1800, 0.8400, -0.9200, -2.9900, 0.8400, -0.9600, -2.7200};

void Initialize_machine_learning()
{
    int err = ei_wrapper_init(result_ready_cb);

    if (err)
    {
        printk("Edge Impulse wrapper failed to initialize (err: %d)\n",
               err);
        return 0;
    };

    if (ARRAY_SIZE(sample_data1) < ei_wrapper_get_window_size())
    {
        printk("Not enough input data\n");
        return 0;
    }

    if (ARRAY_SIZE(sample_data1) % ei_wrapper_get_frame_size() != 0)
    {
        printk("Improper number of input samples\n");
        return 0;
    }

    printk("Machine learning model sampling frequency: %zu\n",
           ei_wrapper_get_classifier_frequency());
    printk("Labels assigned by the model:\n");
    for (size_t i = 0; i < ei_wrapper_get_classifier_label_count(); i++)
    {
        printk("- %s\n", ei_wrapper_get_classifier_label(i));
    }
    printk("\n");

    size_t cnt = 0;


    /* input_data is defined in input_data.h file. */
    err = ei_wrapper_add_data(&sample_data1[cnt],
                              ei_wrapper_get_window_size());
    if (err)
    {
        printk("Cannot provide input data (err: %d)\n", err);
        printk("Increase CONFIG_EI_WRAPPER_DATA_BUF_SIZE\n");
        return 0;
    }
    cnt += ei_wrapper_get_window_size();

    err = ei_wrapper_start_prediction(0, 0);
    if (err)
    {
        printk("Cannot start prediction (err: %d)\n", err);
    }
    else
    {
        printk("Prediction started...\n");
    }

    // /* Predictions for additional data are triggered in the result ready
    //  * callback. The prediction start can be triggered before the input
    //  * data is provided. In that case the prediction is started right
    //  * after the prediction window is filled with data.
    //  */
    // frame_surplus = (ARRAY_SIZE(input_data) - ei_wrapper_get_window_size()) / ei_wrapper_get_frame_size();

    // while (cnt < ARRAY_SIZE(input_data))
    // {
    //     err = ei_wrapper_add_data(&input_data[cnt],
    //                               ei_wrapper_get_frame_size());
    //     if (err)
    //     {
    //         printk("Cannot provide input data (err: %d)\n", err);
    //         printk("Increase CONFIG_EI_WRAPPER_DATA_BUF_SIZE\n");
    //         return 0;
    //     }
    //     cnt += ei_wrapper_get_frame_size();

    //     k_sleep(K_MSEC(FRAME_ADD_INTERVAL_MS));
    // }

    return 0;
}

static void result_ready_cb(int err)
{
    if (err)
    {
        printk("Result ready callback returned error (err: %d)\n", err);
        return;
    }

    const char *label;
    float value;
    float anomaly;

    printk("\nClassification results\n");
    printk("======================\n");

    while (true)
    {
        err = ei_wrapper_get_next_classification_result(&label, &value, NULL);

        if (err)
        {
            if (err == -ENOENT)
            {
                err = 0;
            }
            break;
        }

        printk("Value: %.2f\tLabel: %s\n", value, label);
    }

    if (err)
    {
        printk("Cannot get classification results (err: %d)", err);
    }
    else
    {
        if (ei_wrapper_classifier_has_anomaly())
        {
            err = ei_wrapper_get_anomaly(&anomaly);
            if (err)
            {
                printk("Cannot get anomaly (err: %d)\n", err);
            }
            else
            {
                printk("Anomaly: %.2f\n", anomaly);
            }
        }
    }

    if (frame_surplus > 0)
    {
        err = ei_wrapper_start_prediction(0, 1);
        if (err)
        {
            printk("Cannot restart prediction (err: %d)\n", err);
        }
        else
        {
            printk("Prediction restarted...\n");
        }

        frame_surplus--;
    }
}