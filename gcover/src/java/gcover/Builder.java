/*
 * gcover
 * TODO
 * Created on 14.07.2003
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
 * Builder
 * 
 * @author Tammo van Lessen
 * @version $Id: Builder.java,v 1.2 2004/01/07 20:39:01 squig Exp $
 */
public class Builder {

	public static FileInfo parse(String filename) throws IOException {
		if (filename.indexOf(".gcov") == -1) {
			return null;
		}
		
		System.out.println("Parsing " + filename);

		FileInfo file = new FileInfo(FileUtils.dirname(filename),
			FileUtils.filename(filename.substring(0,filename.lastIndexOf(".gcov"))));
		LineNumberReader lnr = new LineNumberReader(new FileReader(filename));
		String line = lnr.readLine();

		if (line != null) {
			Parser parser;
			if (line.startsWith("        -:    0:")) {
				parser = new GCov333Parser();
			}
			else {
				parser = new DefaultParser();
				LineInfo li = parser.parse(line);
				file.addLine(li);
			}
		
			while ((line = lnr.readLine()) != null) {
				LineInfo li = parser.parse(line);
				if (li != null) {
					file.addLine(li);
				}			
			}
		}
		return file;
	}
	
	public static void parseDir(Project project, String dirname) {
		if (!new File(dirname).isDirectory()) return;
		String[] files = FileUtils.getFilesFromExtension(dirname, new String[] {"gcov"});
		for (int i = 0; i<files.length; i++) {
			try {
				FileInfo file = parse(files[i]);
				Package.getPackage(file.getPackageName()).addFile(file);
				project.addFile(file);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		Package.shortenNames();
	}
	
	private static class GCovFileFilter implements FileFilter {

		/**
		 * @see java.io.FileFilter#accept(java.io.File)
		 */
		public boolean accept(File pathname) {
			return pathname.getName().endsWith(".gcov") 
				|| pathname.isDirectory();
		}
	}

	private static interface Parser {

		public LineInfo parse(String line);

	}

	private static class DefaultParser implements Parser {

		public LineInfo parse(String line)
		{
			LineInfo li = new LineInfo();
			if (line.substring(0, 2).equals("\t\t")) {
				li.setInstrumented(false);
				li.setExecCount(0);
				li.setSourceLine(line.substring(2));
			} else {
				li.setInstrumented(true);
				String linePrefix = line.substring(0, 16).trim();
				int execCount;
				if (linePrefix.equals("######")) {
					execCount = 0;
				} else {
					execCount = Integer.parseInt(linePrefix);
				}
				li.setExecCount(execCount);
				li.setSourceLine(line.substring(16));
			}
			return li;
		}

	}

	private static class GCov333Parser implements Parser {

		public LineInfo parse(String line)
		{
			LineInfo li = new LineInfo();
			StringTokenizer t = new StringTokenizer(line, ":");
			if (t.hasMoreTokens()) {
				String token = t.nextToken().trim();
				if (token.equals("-")) {
					li.setInstrumented(false);
					li.setExecCount(0);
				}
				else {
					li.setInstrumented(true);
					li.setExecCount(token.equals("#####") 
									? 0
									: Integer.parseInt(token));
				}
				
				if (t.hasMoreTokens()) {
					token = t.nextToken().trim();
					int lineNr = Integer.parseInt(token);
					if (lineNr == 0) {
						// skip the meta information
						return null;
					}
				}

				li.setSourceLine(t.nextToken("").substring(1));

				return li;
			}
			return null;
		}

	}

}
