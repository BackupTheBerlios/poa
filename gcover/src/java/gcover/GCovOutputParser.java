/*
 *  GCover - Code coverage report generator for gcov
 *
 *  Copyright (C) 2004 by Steffen Pingel 
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 */
package gcover;

import java.io.File;
import java.io.FileFilter;
import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.util.*;

import org.apache.commons.io.FileUtils;

/**
 * Parses the stdout output of gcov.
 * 
 * @author Steffen Pingel
 * @version $Id: GCovOutputParser.java,v 1.1 2004/01/14 18:44:57 squig Exp $
 */
public class GCovOutputParser {

	/**
	 * 
	 */
	public static void parse(Project project, String filename)
		throws IOException 
	{ 
		LineNumberReader in = new LineNumberReader(new FileReader(filename));
		try {
			String line;
			while ((line = in.readLine()) != null) {
				if (line.indexOf("in file") != -1) {
					int i = line.lastIndexOf(" ");
					FileInfo fileInfo = project.getFile(line.substring(i + 1));
					if (fileInfo != null) {
						try {
							parseLine(line, fileInfo);
						}
						catch (NumberFormatException e) {
						}
					}
				}
			}
		}
		finally {
			in.close();
		}
	}
		
	private static void parseLine(String line, FileInfo file)
	{
		StringTokenizer t = new StringTokenizer(line, " ");
		if (t.countTokens() < 8) {
			return;
		}

		String rateString = t.nextToken();
		if (rateString.endsWith("%")) {
			rateString = rateString.substring(0, rateString.length() - 1);
		}
		double rate = Double.parseDouble(rateString) / 100.0;
		t.nextToken(); // of
		int count = Integer.parseInt(t.nextToken());
		String type = t.nextToken(); // lines | branches | calls
		String action = t.nextToken(); // executed | taken
		String filename;
		if (action.equals("executed")) {
			t.nextToken(); // in
			t.nextToken(); // file
			filename = t.nextToken();
		}
		else if (action.equals("taken") && t.countTokens() >= 5) {
			t.nextToken(); // at
			t.nextToken(); // least
			t.nextToken(); // once
			t.nextToken(); // in
			filename = t.nextToken();
		}
		else {
			return;
		}

		if (type.equals("branches")) {
			if (action.equals("executed")) {
				file.setBranchesExecutedRate(rate);
				file.setBranchesTotal(count);
			}
			else if (action.equals("taken")) {
				file.setBranchesTakenRate(rate);
				file.setBranchesTotal(count);
			}
		}
		else if (type.equals("calls") && action.equals("executed")) {
			file.setCallsExecutedRate(rate);
			file.setCallsTotal(count);
		}
	}
	
}
