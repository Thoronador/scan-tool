# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# Recurse into subdirectory for the main executable tests.
add_subdirectory (scan-tool)

# Recurse into subdirectory for scan-tool-cache tests.
add_subdirectory (scan-tool-cache)

# Recurse into subdirectory for the mso-api-request executable tests.
add_subdirectory (mso-api-request)

# Recurse into subdirectory for the vt-api-request executable tests.
add_subdirectory (vt-api-request)

# Recurse into subdirectory for the honeypottery executable tests.
add_subdirectory (honeypottery)
