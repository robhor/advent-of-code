cc_library(
    name = "day24-lib",
    hdrs = ["day24.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day24",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day24-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day24_test.cc"],
    deps = [
        ":day24-lib",
        "@googletest//:gtest_main",
    ],
)
