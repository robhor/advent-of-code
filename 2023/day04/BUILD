cc_library(
    name = "day04-lib",
    hdrs = ["day04.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
    ],
)

cc_binary(
    name = "day04",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day04-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day04_test.cc"],
    deps = [
        ":day04-lib",
        "@googletest//:gtest_main",
    ],
)
