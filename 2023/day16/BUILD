cc_library(
    name = "day16-lib",
    hdrs = ["day16.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day16",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day16-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day16_test.cc"],
    deps = [
        ":day16-lib",
        "@googletest//:gtest_main",
    ],
)
