#
# Copyright (c) 2012-2013 NEC Corporation
# All rights reserved.
# 
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License v1.0 which accompanies this
# distribution, and is available at http://www.eclipse.org/legal/epl-v10.html
#

##
## Header dependency file.
##

package PFC::Conf::DepFile;

use strict;
use Cwd qw(abs_path);
use FileHandle;
use File::Basename;
use File::Path;
use POSIX;

use constant	PSEUDO_FILE_CHAR	=> '<';

=head1 NAME

PFC::Conf::DepFile - Header dependency file

=head1 SYNOPSIS

  my $file = PFC::Conf::DepFile->new("file.cfdef", "file.c");

  # Add file dependencies.
  $file->add("/usr/include/foo.h");

  # Write make rule.
  $file->output("file.cfdef.d");

=head1 ABSTRACT

B<PFC::Conf::DepFile> is a class associated with file which keeps B<make>
rule describing header file dependency.

Note that B<make> rule itself must be generated by the GNU C preprocessor.

=head1 DESCRIPTION

This section describes about public interface for B<PFC::Conf::DepFile>.

=head2 METHODS

=over 4

=item I<new>($srcfile, $outfile)

Constructor.

=over 4

=item $srcfile

Path to source file.

=item $outfile

Path to file to be generated from the file specified to I<$srcfile>.

=back

=cut

sub new
{
	my $this = shift;
	my $class = ref($this) || $this;
	my ($srcfile, $outfile) = @_;

	return bless({SRCFILE => $srcfile, OUTFILE => $outfile, FMAP => {},
		      FILES => []}, $class);
}

=item I<add>($file)

Add the given file to dependency list.

=cut

sub add
{
	my $me = shift;
	my ($file) = @_;

	# Never add pseudo filename.
	next if (substr($file, 0, 1) eq PSEUDO_FILE_CHAR);

	# Never add file that does not exist.
	my $path = abs_path($file);
	return unless ($path and -f $path);

	my $fmap = $me->{FMAP};
	unless (exists($fmap->{$path})) {
		push(@{$me->{FILES}}, $path);
		$fmap->{$path} = 1;
	}
}

=item I<output>($depfile)

Generate rule for B<make> program describing the header file dependencies.

=cut

sub output
{
	my $me = shift;
	my ($depfile) = @_;

	my $dir = dirname($depfile);
	unless ($dir) {
		# Prepare parent directory.
		mkpath($dir, undef, 0755);
	}

	my $fh = FileHandle->new($depfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	die "open($depfile) failed: $!\n" unless ($fh);

	my $srcfile = $me->{SRCFILE};
	my $outfile = $me->{OUTFILE};
	my $list = $me->{FILES};
	$fh->print($outfile, ": ", join(" \\\n ", $srcfile, @$list), "\n");

	# Write phony target for each dependency.
	foreach my $f (@$list) {
		$fh->print("\n", $f, ":\n");
	}
}

=back

=head1 AUTHOR

NEC Corporation

=cut

1;
