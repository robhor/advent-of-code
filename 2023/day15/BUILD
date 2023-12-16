cc_library(
    name = "day15-lib",
    hdrs = ["day15.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day15",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day15-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day15_test.cc"],
    deps = [
        ":day15-lib",
        "@googletest//:gtest_main",
    ],
)
