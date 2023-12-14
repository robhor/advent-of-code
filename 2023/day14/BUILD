cc_library(
    name = "day14-lib",
    hdrs = ["day14.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day14",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day14-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day14_test.cc"],
    deps = [
        ":day14-lib",
        "@googletest//:gtest_main",
    ],
)
