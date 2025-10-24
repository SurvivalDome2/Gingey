# Error codes for both Ginger, and The Game

The prefix for the Ginger error codes is 2560 and 3515 for The Game.
> These numbers come from converting GR and TG from base 62. Base 62
> being 0-9, a-z, A-Z. Just because I didn't like how base 64 was
> structured.

Note that error codes are given by returning a struct that has the main error,
and also the more detailed code. The more detailed code for GR will start at 1, and the
main error starts at 25600 as stated above. For TG, the detailed code will also start
at 1, and the main error starts at 35150.

When the error detail is a negative number, it means the following:
  <br>&nbsp;&nbsp;&nbsp;&nbsp;-1 - `mainError` is a Vulkan error code

25601 - Failed to initialize the logging thread due to invalid params
  <br>&nbsp;&nbsp;&nbsp;&nbsp;1 - `startAddress` param is 0 (invalid)
  <br>&nbsp;&nbsp;&nbsp;&nbsp;2 - `criticalSection` param is a NULL ptr (invalid)
  <br>&nbsp;&nbsp;&nbsp;&nbsp;3 - `hThread` param is a NULL ptr (invalid)

25602 - Failed to initialize critical section

25603 - Failed to create thread

25604 - Failed to create Vulkan infoInstance due to invalid params
  <br>&nbsp;&nbsp;&nbsp;&nbsp;1 - `name` param is a NULL ptr (invalid)
  <br>&nbsp;&nbsp;&nbsp;&nbsp;2 - `createInfo` param is a NULL ptr (invalid)

25605 - Failed to allocate memory to the extensions list in `createInfoInstance`

25606 - Found no suitable GPUs for Vulkan