cc_library(
    name = "day22-lib",
    hdrs = ["day22.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day22",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day22-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day22_test.cc"],
    deps = [
        ":day22-lib",
        "@googletest//:gtest_main",
    ],
)
