cc_library(
    name = "day23-lib",
    hdrs = ["day23.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day23",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day23-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day23_test.cc"],
    deps = [
        ":day23-lib",
        "@googletest//:gtest_main",
    ],
)
