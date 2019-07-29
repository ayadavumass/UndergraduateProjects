# #
# #  regression tests for each component
# #
# all: lex parse sa cg gc
# 	echo "Tests completed."

# install:
# 	perl install_results
# lex:
# 	perl test_lexer

# parse:
# 	perl test_parser

# sa:
# 	perl test_sa

# cg:
# 	perl test_cg

# gc:
# 	perl test_gc
# #
# # clean up the results of the last test
# #
# # Delete core, *~, and all ./RESULTS_<whatever>/*, except CVS directories.
# #
clean:
	rm -f core *~;
	find RESULTS_* -name 'CVS' -prune -type f -o -type f -exec rm {} \; ;

