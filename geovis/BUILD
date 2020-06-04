load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
	name = "geovis",
	srcs = glob(["*.h", "*.cpp"]),
	copts = ["-Wall", "-std=c++1z"],
	linkopts = ["-L/usr/local/lib/", "-lsigil", "-lGLESv2"],
)
