# fuse-example

# Before you start using it
After downloading this repo, you have to make sure that
all its dependencies are up to date. Run:
`
git submodule update --remote
`

To compile the project, run:
`
make fuse-example
`

# To run the project
Make sure that you you have created a mounting point (directory)
and you have the right "rights".

Then, in one terminal you should see all the debugging messages
by running:
`
./fuse\_example -f $MOUNTING\_POINT
`
Access the $MOUNTING\_POINT as it was any other type of directory.

# The sweet end
After you are done, you can press Ctrl-C to stop execution, or
from another terminal run: `sudo umount $MOUNTING\_POINT`.

# References
* [libfuse](https://github.com/libfuse/libfuse)
* [memfs-fuse](https://github.com/tniessen/memfs-fuse)
* [stat man page](https://man7.org/linux/man-pages/man2/stat.2.html)


