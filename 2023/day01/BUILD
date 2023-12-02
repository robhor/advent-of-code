cc_library(
    name = "day01-lib",
    hdrs = ["day01.cc"],
)

cc_binary(
    name = "day01",
    srcs = ["main.cc"],
    data = ["input.txt"],
    deps = [
        ":day01-lib",
    ],
)

cc_test(
    name = "test",
    srcs = ["day01_test.cc"],
    deps = [
        ":day01-lib",
        "@com_google_googletest//:gtest_main",
    ],
)
