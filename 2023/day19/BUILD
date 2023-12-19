cc_library(
    name = "day19-lib",
    hdrs = ["day19.cc"],
    deps = [
        "@abseil-cpp//absl/strings",
        "@range-v3",
    ],
)

cc_binary(
    name = "day19",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day19-lib",
    ],
)

cc_test(
    name = "test",
    data = ["input.txt"],
    srcs = ["day19_test.cc"],
    deps = [
        ":day19-lib",
        "@googletest//:gtest_main",
    ],
)
