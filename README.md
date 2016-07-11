# The Corax Operating System

Corax is an experimental microkernel for Intel IA-32 and Intel64 processor
architectures. It is designed to be minimalist, efficient, and scalable.

Since the kernel is still far from usable (you can boot it, but it won't do
anything useful yet), there is not much in terms of documentation and design
overviews. However, I am aiming to keep the kernel as small as possible, leaving
out functionality that I don't deem necessary (for example, swapping of main
memory or uncommon protocols such as IPX or AppleTalk). The kernel shouldn't do
much, but it should be good at the few things it can do.

When it comes to the API, I am trying to be as POSIX-compliant as possible, but
I don't mind sacrificing compatibility where it makes sense for security.


## Compiling

To compile the kernel, simply execute `make` in the project root:

    $ make

The resulting image will be called *corax*. It can be booted with grub by adding
an entry like the following to your `grub.cfg` file:

    menuentry 'Corax 0.1 unstable/development' --class os {
        insmod ext2
        set root=(hd0,1)
        multiboot /path/to/corax
        boot
    }

Make sure to change the filesystem module, root name, and kernel path according
to your setup. Once you're done, it should be possible to boot Corax on your
machine.
**Remember**: this is an experimental kernel and should you decide to use it,
you are doing so at your own risk.


## License ##

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
