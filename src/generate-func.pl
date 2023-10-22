#!/usr/bin/perl
use strict;
use warnings;

usage();

my $G_FNAME = $ARGV[0];
my $G_DIR_C = 'jstr';

my $G_NMSPC     = 'jstr';
my $G_NMSPC_UPP = uc($G_NMSPC);
my $G_STR_STRCT = 'jstr_ty';
my $G_STRCT_VAR = 'j';

my $G_STRCT_DATA = 'data';
my $G_STRCT_SIZE = 'size';
my $G_STRCT_CAP  = 'capacity';

my $G_CAP_VAR  = 'cap';
my $G_SIZE_VAR = 'sz';
my $G_LEN_VAR  = 'len';

my $G_MCR_INLINE          = $G_NMSPC_UPP . '_INLINE';
my $G_MCR_MAYBE_UNUSED    = $G_NMSPC_UPP . '_MAYBE_UNUSED';
my $G_MCR_RESTRICT        = $G_NMSPC_UPP . '_RESTRICT';
my $G_MCR_WARN_UNUSED     = $G_NMSPC_UPP . '_WARN_UNUSED';
my $G_MCR_RETURNS_NONNULL = $G_NMSPC_UPP . '_RETURNS_NONNULL';

my $G_RE_FN     = qr/[ \t]*((?:\/\*|\/\/|$G_NMSPC_UPP\_|static)\s+\w+\s+(\w*(?:$G_NMSPC)\_.*?)\(((?:.|\n)*?\)[^{]*))/;
my $G_RE_DEFINE = qr/\([^)]*\)[^{]*{[^}]*}/;

my $G_LEN_FN_SUFFIX = '_len';

my $g_in_header = get_file_str($G_FNAME);
$g_in_header = tidy_newlines($g_in_header);

my @LINES      = gen_nonlen_funcs($g_in_header);
my $out_header = gen_struct_funcs(@LINES);

print $out_header;

sub usage
{
	if ($#ARGV != 0) {
		print('Usage: ./gen_func.pl <file>');
		exit;
	}
}

sub get_file_str
{
	my ($FILENAME) = @_;
	my $in_header = '';
	my @lines;
	open(my $FH, '<', $FILENAME)
	  or die("Can't open $FILENAME\n");
	while (<$FH>) {
		$in_header .= $_;
		push(@lines, $_);
	}
	close($FH);
	my @def;
	my @undef;

	# prefix temporary macros with PJSTR_ to avoid naming conflicts
	foreach (@lines) {
		if (/^[ \t]*#[ \t]*undef[ \t]*([_A-Z0-9]*)/) {
			my $macro = $1;
			if ($macro !~ /^(?:PJSTR|JSTR|pjstr|jstr)/) {
				$in_header =~ s/([^'"_0-9A-Za-z]|^)$macro([^'"_0-9A-Za-z]|$)/$1PJSTR_$macro$2/g;
			}
		}
	}
	return $in_header;
}

sub tidy_newlines
{
	my ($in_header) = @_;
	$in_header =~ s/(\n[ \t].*)\n\n\n*/$1\n/g;
	$in_header =~ s/\n\n\n*\t/\n\t/g;
	$in_header =~ s/\n\n\n* /\n /g;
	return $in_header;
}

sub gen_nonlen_funcs
{
	my ($FILE_STR) = @_;
	my @OLD_LINES = split(/\n\n/, $FILE_STR);
	my @new_lines;
	foreach (@OLD_LINES) {
		if ($_ !~ $G_RE_FN) {
			goto CONT;
		}
		my $decl      = $1;
		my $FUNC_NAME = $2;
		my $params    = $3;
		if (   (!$decl && !$FUNC_NAME && !$params)
			|| ($FUNC_NAME !~ /$G_NMSPC[_0-9_A-Za-z]*$G_LEN_FN_SUFFIX(?:_|$)/o))
		{
			goto CONT;
		}
		my $tmp = $FUNC_NAME;
		$tmp =~ s/$G_LEN_FN_SUFFIX//o;
		if (   ($g_in_header =~ /$tmp\(/)
			|| ($_         !~ $G_RE_DEFINE)
			|| ($FUNC_NAME =~ /^p/))
		{
			goto CONT;
		}
		$decl = add_inline($decl);

		# my $PTR    = ($decl =~ /\*.*\*/)         ? '&'       : '';
		my $RETURN = (index($_, 'return') != -1) ? 'return ' : '';
		$params =~ s/\)/,/;
		my @RAW_ARGS = split(/\s/, $params);
		my @clean_args;
		foreach (@RAW_ARGS) {
			if (/,$/) {
				s/,//;
				push(@clean_args, $_);
			}
		}
		$decl =~ s/($G_NMSPC\_\w*)$G_LEN_FN_SUFFIX(\w*\()/$1$2/o;
		$decl .= "\n{\n\t";
		my $size_ptr_var = get_size_ptr($FUNC_NAME, $params);
		$decl .= "$RETURN$FUNC_NAME(";
		my $LEN = (index($params, $G_LEN_VAR) != -1) ? 1 : 0;
		foreach (@clean_args) {
			if ($LEN) {
				if (/(\w*)$G_LEN_VAR/) {
					my $var = $1;
					$decl =~ s/,[ \t\n_0-9A-Za-z]*$G_LEN_VAR//o;
					$_ = "strlen($var)";
				}
			} else {
				if (!$size_ptr_var && /\w*$G_SIZE_VAR/) {
					$decl =~ s/,[ \t\n_0-9A-Za-z]*$G_SIZE_VAR//o;
					$_ = "strlen($clean_args[0])";
				}
			}
			$decl .= "$_, ";
		}
		$decl =~ s/, $//;
		$decl .= ");\n}\n";
		push(@new_lines, $_);
		push(@new_lines, $decl);
		next;
	  CONT:
		push(@new_lines, $_);
	}
	return @new_lines;
}

sub gen_struct_funcs
{
	my (@LINES) = @_;
	my $out_header;
	foreach (@LINES) {
		if ($_ !~ $G_RE_FN) {
			goto CONT;
		}
		my $decl      = $1;
		my $FUNC_NAME = $2;
		my $params    = $3;
		if (!$decl && !$FUNC_NAME && !$params) {
			goto CONT;
		}
		$params =~ s/\)/,/;
		my $HAS_SZ  = ($params =~ /$G_SIZE_VAR(?:,|\))/o) ? 1 : 0;
		my $HAS_CAP = ($params =~ /$G_CAP_VAR(?:,|\))/o)  ? 1 : 0;
		if (   (!$HAS_SZ && !$HAS_CAP)
			|| (index($params, "...") != -1)
			|| ($_         !~ $G_RE_DEFINE)
			|| ($FUNC_NAME =~ /^p/)
			|| (index($g_in_header, "$FUNC_NAME\_j") != -1))
		{
			goto CONT;
		}
		my $RETURN = (index($decl, 'void') != -1) ? '' : 'return ';
		if ($RETURN) {
			$decl =~ s/$G_MCR_RETURNS_NONNULL//;
		}
		my $RETURNS_END_PTR = 0;
		$decl =~ s/$FUNC_NAME/$FUNC_NAME\_j/;
		$decl = add_inline($decl);
		if ($FUNC_NAME =~ /_p(?:_|$)/) {
			$decl =~ s/.*Return value:(?:.|\n)*?(\*\/|\/\/)/$1/;
			$decl =~ s/_p//;
			$decl =~ s/static\s*(?:char|void)\s\*/static void /;
			$decl =~ s/[ \t]*$G_MCR_WARN_UNUSED[ \t]*\n//o;
			$RETURN          = '';
			$RETURNS_END_PTR = 1;
		}
		my $PTR = ($decl =~ /\([^,)]*\*.*\*/) ? '&' : '';
		if ($HAS_SZ && $decl =~ /\w*$G_SIZE_VAR(,|\))/o) {
			if ($1 eq ')') {
				$decl =~ s/[^(,]*$G_SIZE_VAR(?:,|\))/)/o;
			} elsif ($1 eq ',') {
				$decl =~ s/[^(,]*$G_SIZE_VAR,//o;
			}
		}
		if ($HAS_CAP && $decl =~ /\w*\s*$G_CAP_VAR(,|\))/o) {
			if ($1 eq ')') {
				$decl =~ s/[^(,]*\s*$G_CAP_VAR(?:,|\))/)/o;
			} elsif ($1 eq ',') {
				$decl =~ s/[^(,]*\s*$G_CAP_VAR,//o;
			}
		}
		my @RAW_ARGS        = split(/\s/, $params);
		my $CONST           = (index($RAW_ARGS[0], 'const') != -1)     ? 'const ' : '';
		my $CONST_STRCT_PTR = ($params =~ /[^\n]*const[^\n]*(?:,|\))/) ? 'const ' : '';
		{
			my $LAST = (index($params, ',') != -1) ? ',' : ')';
			my $tmp  = "($CONST$G_STR_STRCT *$G_MCR_RESTRICT $CONST_STRCT_PTR" . "j$LAST";
			$decl =~ s/\(.+?$LAST/$tmp/;
		}
		$decl =~ s/,\s*\)/)/g;
		$decl .= "\n{\n\t";
		my $func_body = "$RETURN$FUNC_NAME(";
		my @clean_args;

		foreach (@RAW_ARGS) {
			if (/,$/) {
				s/,//;
				push(@clean_args, $_);
			}
		}
		if (using_str_ptr($params)) {
			$func_body .= $PTR;
		}
		$func_body .= "$G_STRCT_VAR->$G_STRCT_DATA, ";
		for (my $i = 1 ; $i <= $#clean_args ; ++$i) {
			if (index($clean_args[$i], $G_SIZE_VAR) != -1) {
				if ($FUNC_NAME =~ /$G_NMSPC/ && using_size_ptr($params)) {
					$clean_args[$i] = "&$G_STRCT_VAR->$G_STRCT_SIZE";
				} else {
					$clean_args[$i] = $PTR . "$G_STRCT_VAR->$G_STRCT_SIZE";
				}
			} elsif ($clean_args[$i] eq $G_CAP_VAR) {
				$clean_args[$i] = $PTR . "$G_STRCT_VAR->$G_STRCT_CAP";
			}
			$func_body .= "$clean_args[$i], ";
		}
		$func_body =~ s/, $//;
		$func_body .= ")";
		if ($RETURNS_END_PTR) {
			$func_body = "$G_STRCT_VAR->$G_STRCT_SIZE = $func_body";
			$func_body .= " - $G_STRCT_VAR->$G_STRCT_DATA";
		}
		$decl .= "$func_body;\n}\n";
		$decl =~ s/\n\n/\n/g;
		$_          .= "\n\n";
		$decl       .= "\n\n";
		$out_header .= $_;
		$out_header .= $decl;
		next;
	  CONT:
		$_          .= "\n\n";
		$out_header .= $_;
	}
	$out_header =~ s/\n\n\n/\n\n/g;
	$out_header =~ s/\n\n*$/\n/g;
	return $out_header;
}

sub get_size_ptr
{
	my ($fn, $params) = @_;
	if (index($fn, $G_NMSPC) != -1) {
		$params =~ /\*.*\*.*(\w*sz)/o;
		return $1;
	}
	return 0;
}

sub using_str_ptr
{
	my ($params) = @_;
	return ($params =~ /\*.*\*.*s[,\)]/o) ? 1 : 0;
}

sub using_size_ptr
{
	my ($params) = @_;
	return ($params =~ /\*.*sz[,\)]/) ? 1 : 0;
}

sub add_inline
{
	my ($decl) = @_;
	if ($decl !~ /$G_MCR_INLINE[^_]/o) {
		$decl =~ s/static/$G_MCR_INLINE\n$G_MCR_MAYBE_UNUSED\nstatic/o;
	}
	return $decl;
}
