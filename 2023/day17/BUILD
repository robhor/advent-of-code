cc_library(
    name = "day17-lib",
    hdrs = ["day17.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day17",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day17-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day17_test.cc"],
    deps = [
        ":day17-lib",
        "@googletest//:gtest_main",
    ],
)
