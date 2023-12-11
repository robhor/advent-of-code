cc_library(
    name = "day11-lib",
    hdrs = ["day11.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day11",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day11-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day11_test.cc"],
    deps = [
        ":day11-lib",
        "@googletest//:gtest_main",
    ],
)
