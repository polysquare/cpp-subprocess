#!/usr/bin/tclsh
# Curly brackets from the same pair should be either in the same line or in the same column

proc acceptPairs {} {
    global file parens index end vitiatingLine vitiatingColumn

    while {$index != $end} {
        set nextToken [lindex $parens $index]
        set tokenValue [lindex $nextToken 0]

        set line [lindex $nextToken 1]

        set processingIndex $index
        incr index

        # Initially set vitiatingLine and vitiatingColumn
        # to -1 and if we find a token which might indicate the
        # existence of either an EXPECT_* or ASSERT_* or
        # or a lambda then set it to the current
        # line and use that to override the position of the
        # leftmost paren
        set expectation [string last "EXPECT_" $tokenValue]
        set assertion [string last "ASSERT_" $tokenValue]

        if {$expectation != -1 || $assertion != -1} {
            set vitiatingLine [lindex $nextToken 1]
            set vitiatingColumn [lindex $nextToken 2]
            continue
        } elseif {$tokenValue == "\["} {
            # If we hit a lambda declaration, we need
            # to go back to either the first (
            # of that line or the first token
            # of that line and use that as the column
            set vitiatingLine [lindex $nextToken 1]
            set currentIndexOnLine $processingIndex
            set evaluatingIndex $currentIndexOnLine

            while {"true"} {
                set previousIndex [expr {$evaluatingIndex - 1}]
                set previousToken [lindex $parens $previousIndex]
                set previousLine [lindex $previousToken 1]
                set previousValue [lindex $previousToken 0]
                set previousType [lindex $previousToken 3]

                # Always decrement evaluatingIndex
                set evaluatingIndex $previousIndex

                if {$previousLine != $vitiatingLine} {
                    break;
                } elseif {$previousType == "space"} {
                    # Ignore spaces
                    continue;
                } elseif {$previousType == "newline"} {
                    break;
                } elseif {$previousValue == "\("} {
                    break;
                }

                # The is the index we actually want to revert
                # back to later
                set currentIndexOnLine $evaluatingIndex
            }

            set currentTokenOnLine [lindex $parens $currentIndexOnLine]
            set vitiatingColumn [lindex $currentTokenOnLine 2]
            continue
        }

        if {$tokenValue == "\{"} {

            set leftParenLine [lindex $nextToken 1]
            set leftParentColumn 0

            if {$leftParenLine == $vitiatingLine} {
                set leftParenColumn ${vitiatingColumn}
            } else {
                set leftParenColumn [lindex $nextToken 2]
            }

            # Now that we have a paren and we've either
            # substituted the vitiating line or column
            # we can reset these to -1
            set vitiatingLine -1
            set vitiatingColumn -1

            acceptPairs

            if {$index == $end} {
                #ignore EOF whitespace
                set thisToken [lindex $parens $index]
                set thisTokenValue [lindex $thisToken 3]
                if {$thisTokenValue == ""} {
                    return
                }

                report $file $leftParenLine "opening curly bracket is not closed"
                return
            }

            # The index will be incremented past the
            # end paren at this point, so we need to
            # use the last one
            set prevIndex [expr {$index - 1}]
            set nextToken [lindex $parens $prevIndex]
            set tokenValue [lindex $nextToken 0]

            # tokenValue must be a right brace
            if {$tokenValue == "\}"} {

                set rightParenLine [lindex $nextToken 1]
                set rightParenColumn [lindex $nextToken 2]

                if {($leftParenLine != $rightParenLine) && ($leftParenColumn != $rightParenColumn)} {
                    # make an exception for line continuation
                    set leftLine [getLine $file $leftParenLine]
                    set rightLine [getLine $file $rightParenLine]
                    if {[string index $leftLine end] != "\\" && [string index $rightLine end] != "\\"} {
                        report $file $rightParenLine "closing curly bracket not in the same line or column"
                    }
                }
            }
        } elseif {$tokenValue == "\}"} {
            return
        }
    }
}

foreach file [getSourceFileNames] {
    set parens [getTokens $file 1 0 -1 -1 {}]
    set index 0
    set vitiatingLine -1
    set vitiatingColumn -1
    set end [llength $parens]
    acceptPairs
    if {$index != $end} {
        report $file [lindex [lindex $parens $index] 1] "excessive closing bracket?"
    }
}
