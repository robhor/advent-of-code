cc_library(
    name = "day10-lib",
    hdrs = ["day10.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day10",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day10-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day10_test.cc"],
    deps = [
        ":day10-lib",
        "@googletest//:gtest_main",
    ],
)
