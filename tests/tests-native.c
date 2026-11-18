#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../src/turbores.h"

#define DONT_CHECK -1

typedef struct TestCase {
    const char *name;
    const char *packet_path;
    const char *reference_path;
    uint32_t bit_depth;
    uint32_t visible_width;
    uint32_t visible_height;
    uint32_t coded_width;
    uint32_t coded_height;
    uint32_t pixel_format;
    int32_t aspect_ratio_num;
    int32_t aspect_ratio_den;
    int32_t color_primaries;
    int32_t color_transfer;
    int32_t color_matrix;
    uint32_t scan_type;
    size_t frame_data_size;
} TestCase;

static const TestCase test_cases[] = {
    {
        .name = "Full HD 422 frame",
        .packet_path = "tests/public/buck-bunny.prores",
        .reference_path = "build/test-references/buck-bunny.framedata",
        .bit_depth = 10,
        .visible_width = 1920,
        .visible_height = 1080,
        .coded_width = 1920,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I422P10,
        .aspect_ratio_num = 1,
        .aspect_ratio_den = 1,
        .color_primaries = 1,
        .color_transfer = 1,
        .color_matrix = 1,
        .scan_type = TURBORES_SCAN_TYPE_PROGRESSIVE,
        .frame_data_size = (size_t) 1920 * 1088 * 2 * 2,
    },
    {
        .name = "1904-wide frame",
        .packet_path = "tests/public/buck-bunny-1904.prores",
        .reference_path = "build/test-references/buck-bunny-1904.framedata",
        .bit_depth = 10,
        .visible_width = 1904,
        .visible_height = 1080,
        .coded_width = 1904,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I422P10,
        .aspect_ratio_num = DONT_CHECK,
        .aspect_ratio_den = DONT_CHECK,
        .color_primaries = DONT_CHECK,
        .color_transfer = DONT_CHECK,
        .color_matrix = DONT_CHECK,
        .scan_type = TURBORES_SCAN_TYPE_PROGRESSIVE,
        .frame_data_size = (size_t) 1904 * 1088 * 2 * 2,
    },
    {
        .name = "444 frame",
        .packet_path = "tests/public/buck-bunny-444.prores",
        .reference_path = "build/test-references/buck-bunny-444.framedata",
        .bit_depth = 10,
        .visible_width = 1904,
        .visible_height = 1080,
        .coded_width = 1904,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I444P10,
        .aspect_ratio_num = DONT_CHECK,
        .aspect_ratio_den = DONT_CHECK,
        .color_primaries = DONT_CHECK,
        .color_transfer = DONT_CHECK,
        .color_matrix = DONT_CHECK,
        .scan_type = TURBORES_SCAN_TYPE_PROGRESSIVE,
        .frame_data_size = (size_t) 1904 * 1088 * 3 * 2,
    },
    {
        .name = "Transparent frame",
        .packet_path = "tests/public/transparent.prores",
        .reference_path = "build/test-references/transparent.framedata",
        .bit_depth = 10,
        .visible_width = 1904,
        .visible_height = 1080,
        .coded_width = 1904,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I444AP10,
        .aspect_ratio_num = DONT_CHECK,
        .aspect_ratio_den = DONT_CHECK,
        .color_primaries = 2,
        .color_transfer = 2,
        .color_matrix = 2,
        .scan_type = TURBORES_SCAN_TYPE_PROGRESSIVE,
        .frame_data_size = (size_t) 1904 * 1088 * 4 * 2,
    },
    {
        .name = "12-bit transparent frame",
        .packet_path = "tests/public/4444-12bit.prores",
        .reference_path = "build/test-references/4444-12bit.framedata",
        .bit_depth = 12,
        .visible_width = 1920,
        .visible_height = 1080,
        .coded_width = 1920,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I444AP12,
        .aspect_ratio_num = DONT_CHECK,
        .aspect_ratio_den = DONT_CHECK,
        .color_primaries = DONT_CHECK,
        .color_transfer = DONT_CHECK,
        .color_matrix = DONT_CHECK,
        .scan_type = TURBORES_SCAN_TYPE_PROGRESSIVE,
        .frame_data_size = (size_t) 1920 * 1088 * 4 * 2,
    },
    {
        .name = "Interlaced frame",
        .packet_path = "tests/public/interlaced-buck-bunny.prores",
        .reference_path = "build/test-references/interlaced-buck-bunny.framedata",
        .bit_depth = 10,
        .visible_width = 1920,
        .visible_height = 1080,
        .coded_width = 1920,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I422P10,
        .aspect_ratio_num = DONT_CHECK,
        .aspect_ratio_den = DONT_CHECK,
        .color_primaries = DONT_CHECK,
        .color_transfer = DONT_CHECK,
        .color_matrix = DONT_CHECK,
        .scan_type = TURBORES_SCAN_TYPE_INTERLACED_TOP_FIELD_FIRST,
        .frame_data_size = (size_t) 1920 * 1088 * 2 * 2,
    },
    {
        .name = "HDR 422 frame",
        .packet_path = "tests/public/hdr-422.prores",
        .reference_path = "build/test-references/hdr-422.framedata",
        .bit_depth = 10,
        .visible_width = 1920,
        .visible_height = 1080,
        .coded_width = 1920,
        .coded_height = 1088,
        .pixel_format = TURBORES_PIXEL_FORMAT_I422P10,
        .aspect_ratio_num = 1,
        .aspect_ratio_den = 1,
        .color_primaries = 9,
        .color_transfer = 18,
        .color_matrix = 9,
        .scan_type = TURBORES_SCAN_TYPE_PROGRESSIVE,
        .frame_data_size = (size_t) 1920 * 1088 * 2 * 2,
    },
};

#define CHECK_EQ(actual, expected, what) \
    do { \
        long long check_actual = (long long) (actual); \
        long long check_expected = (long long) (expected); \
        if (check_actual != check_expected) { \
            fprintf( \
                stderr, \
                "FAILED: %s (threads %u): %s was %lld, expected %lld\n", \
                test_case->name, threads, what, check_actual, check_expected \
            ); \
            return 1; \
        } \
    } while (0)

static uint8_t *readFile(const char *path, size_t *size) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open %s\n", path);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    *size = (size_t) ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t *data = malloc(*size);
    if (fread(data, 1, *size, file) != *size) {
        fprintf(stderr, "Failed to read %s\n", path);
        exit(1);
    }

    fclose(file);
    return data;
}

static int runCase(const TestCase *test_case, uint32_t threads) {
    size_t packet_size;
    uint8_t *packet = readFile(test_case->packet_path, &packet_size);

    size_t reference_size;
    uint8_t *reference = readFile(test_case->reference_path, &reference_size);

    TurboresDecoder *decoder = turbores_decoder_create(threads, test_case->bit_depth, TURBORES_ALL_PIXEL_FORMATS);
    TurboresFrame *frame = turbores_frame_create();

    int32_t result = turbores_decode(decoder, frame, packet, packet_size);
    if (result != 0) {
        const uint8_t *message = turbores_decoder_error_message_ptr(decoder);
        fprintf(
            stderr,
            "FAILED: %s (threads %u): turbores_decode returned %d: %.*s\n",
            test_case->name,
            threads,
            result,
            (int) turbores_decoder_error_message_size(decoder),
            message ? (const char *) message : ""
        );
        return 1;
    }

    CHECK_EQ(turbores_frame_visible_width(frame), test_case->visible_width, "visible width");
    CHECK_EQ(turbores_frame_visible_height(frame), test_case->visible_height, "visible height");
    CHECK_EQ(turbores_frame_coded_width(frame), test_case->coded_width, "coded width");
    CHECK_EQ(turbores_frame_coded_height(frame), test_case->coded_height, "coded height");
    CHECK_EQ(turbores_frame_pixel_format(frame), test_case->pixel_format, "pixel format");
    CHECK_EQ(turbores_frame_original_pixel_format(frame), test_case->pixel_format, "original pixel format");
    CHECK_EQ(turbores_frame_scan_type(frame), test_case->scan_type, "scan type");

    if (test_case->aspect_ratio_num != DONT_CHECK) {
        CHECK_EQ(turbores_frame_aspect_ratio_num(frame), test_case->aspect_ratio_num, "aspect ratio numerator");
        CHECK_EQ(turbores_frame_aspect_ratio_den(frame), test_case->aspect_ratio_den, "aspect ratio denominator");
    }
    if (test_case->color_primaries != DONT_CHECK) {
        CHECK_EQ(turbores_frame_color_primaries(frame), test_case->color_primaries, "color primaries");
        CHECK_EQ(turbores_frame_color_transfer(frame), test_case->color_transfer, "color transfer");
        CHECK_EQ(turbores_frame_color_matrix(frame), test_case->color_matrix, "color matrix");
    }

    CHECK_EQ(turbores_frame_data_size(frame), test_case->frame_data_size, "frame data size");
    CHECK_EQ(reference_size, test_case->frame_data_size, "reference size");

    if (memcmp(turbores_frame_data_ptr(frame), reference, reference_size) != 0) {
        fprintf(stderr, "FAILED: %s (threads %u): frame data differs from reference\n", test_case->name, threads);
        return 1;
    }

    turbores_frame_destroy(frame);
    turbores_decoder_destroy(decoder);
    free(packet);
    free(reference);

    printf("ok - %s (threads %u)\n", test_case->name, threads);
    return 0;
}

/*
 * Reduced-resolution decoding: every plane must have the reduced dimensions, a 1/8 decode must equal the mean of each
 * full-resolution 8x8 block (it is exactly DC), and 1/2 and 1/4 decodes must stay close to a box downscale.
 */
static int runDownscaleCase(const TestCase *test_case, uint32_t threads) {
    size_t packet_size;
    uint8_t *packet = readFile(test_case->packet_path, &packet_size);
    if (!packet) {
        fprintf(stderr, "FAILED: %s: could not read packet\n", test_case->name);
        return 1;
    }

    TurboresDecoder *decoder = turbores_decoder_create(threads, test_case->bit_depth, TURBORES_ALL_PIXEL_FORMATS);
    TurboresFrame *full = turbores_frame_create();
    int failures = 0;

    if (turbores_decode(decoder, full, packet, packet_size) != 0) {
        fprintf(stderr, "FAILED: %s downscale: full decode failed\n", test_case->name);
        failures++;
        goto done;
    }

    if (turbores_decoder_set_downscale(decoder, 4) != TURBORES_ERROR_NOT_SUPPORTED) {
        fprintf(stderr, "FAILED: %s downscale: 4 was accepted\n", test_case->name);
        failures++;
    }

    const uint32_t width = turbores_frame_coded_width(full);
    const uint32_t height = turbores_frame_coded_height(full);
    const uint16_t *full_luma = (const uint16_t *) turbores_frame_data_ptr(full);
    // A box downscale and a truncated DCT are different filters, so 1/2 and 1/4 only get a loose bound
    const double max_rmse[4] = { 0, 24.0, 24.0, 1.0 };

    for (uint32_t log2_downscale = 1; log2_downscale <= 3; log2_downscale++) {
        const uint32_t s = 1u << log2_downscale;
        TurboresFrame *reduced = turbores_frame_create();
        turbores_decoder_set_downscale(decoder, log2_downscale);
        int32_t result = turbores_decode(decoder, reduced, packet, packet_size);
        turbores_decoder_set_downscale(decoder, 0);

        if (result != 0) {
            fprintf(stderr, "FAILED: %s 1/%u (threads %u): decode returned %d\n", test_case->name, s, threads, result);
            failures++;
            turbores_frame_destroy(reduced);
            continue;
        }

        if (turbores_frame_coded_width(reduced) != width / s || turbores_frame_coded_height(reduced) != height / s ||
            turbores_frame_visible_width(reduced) != (test_case->visible_width + s - 1) / s ||
            turbores_frame_visible_height(reduced) != (test_case->visible_height + s - 1) / s ||
            turbores_frame_data_size(reduced) * s * s != turbores_frame_data_size(full) ||
            turbores_frame_pixel_format(reduced) != turbores_frame_pixel_format(full)) {
            fprintf(stderr, "FAILED: %s 1/%u (threads %u): wrong frame geometry\n", test_case->name, s, threads);
            failures++;
            turbores_frame_destroy(reduced);
            continue;
        }

        // Luma against a box downscale of the full decode. Interlaced frames downscale each field on its own, which
        // a box over interleaved rows does not, so they only get the geometry checks.
        if (test_case->scan_type == TURBORES_SCAN_TYPE_PROGRESSIVE) {
            const uint16_t *reduced_luma = (const uint16_t *) turbores_frame_data_ptr(reduced);
            double squared_error = 0;
            for (uint32_t y = 0; y < height / s; y++) {
                for (uint32_t x = 0; x < width / s; x++) {
                    double sum = 0;
                    for (uint32_t dy = 0; dy < s; dy++) {
                        for (uint32_t dx = 0; dx < s; dx++) {
                            sum += full_luma[(size_t) (y * s + dy) * width + x * s + dx];
                        }
                    }
                    const double error = reduced_luma[(size_t) y * (width / s) + x] - sum / (s * s);
                    squared_error += error * error;
                }
            }
            const double rmse = sqrt(squared_error / ((double) (width / s) * (height / s)));
            if (rmse > max_rmse[log2_downscale]) {
                fprintf(
                    stderr,
                    "FAILED: %s 1/%u (threads %u): luma RMSE %.2f against a box downscale (limit %.2f)\n",
                    test_case->name, s, threads, rmse, max_rmse[log2_downscale]
                );
                failures++;
            }
        }

        turbores_frame_destroy(reduced);
    }

    if (failures == 0) {
        printf("ok - %s downscale 1/2, 1/4, 1/8 (threads %u)\n", test_case->name, threads);
    }

done:
    turbores_frame_destroy(full);
    turbores_decoder_destroy(decoder);
    free(packet);
    return failures;
}

int main(void) {
    const uint32_t thread_configs[] = { 0, 4 };
    int failures = 0;

    for (size_t t = 0; t < sizeof(thread_configs) / sizeof(thread_configs[0]); t++) {
        for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
            failures += runCase(&test_cases[i], thread_configs[t]);
        }
        for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
            failures += runDownscaleCase(&test_cases[i], thread_configs[t]);
        }
    }

    if (failures > 0) {
        fprintf(stderr, "%d test(s) failed\n", failures);
        return 1;
    }

    printf("\nAll tests passed.\n");
    return 0;
}
