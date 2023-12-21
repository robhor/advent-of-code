cc_library(
    name = "day21-lib",
    hdrs = ["day21.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day21",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day21-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day21_test.cc"],
    deps = [
        ":day21-lib",
        "@googletest//:gtest_main",
    ],
)
