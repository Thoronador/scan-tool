# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# Recurse into subdirectory for the main executable.
add_subdirectory (scan-tool)

# Recurse into subdirectory for scan-tool-cache.
add_subdirectory (scan-tool-cache)

# Recurse into subdirectory for the main executable.
add_subdirectory (scan-tool-mso)

# Recurse into subdirectory for the honeypottery executable.
add_subdirectory (honeypottery)

# Recurse into subdirectory for mso-api-request executable.
add_subdirectory (mso-api-request)

# Recurse into subdirectory for vt-api-request executable.
add_subdirectory (vt-api-request)
