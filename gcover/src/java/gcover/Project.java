/*
 * gcover
 * TODO
 * Created on 14.07.2003
 *
 */
package gcover;

import java.util.ArrayList;
import java.util.*;
import java.util.List;
import java.util.Map;

/**
 * Project
 * 
 * @author Tammo van Lessen
 * @version $Id: Project.java,v 1.3 2004/01/11 16:01:34 squig Exp $
 */
public class Project {

	private List files = new ArrayList();
	private Map packages = new HashMap();
	private long instrumentedLines = 0;
	private long executedLines = 0;
	private long linesOfCode = 0;

	public int getFileCount()
	{
		return files.size();
	}

	public FileInfo[] getFiles() {
		FileInfo[] array = (FileInfo[])files.toArray(new FileInfo[0]);
		Arrays.sort(array);
		return array;
	}
	
	public void addFile(FileInfo file) {
		files.add(file);
		instrumentedLines += file.getInstrumentedLinesCount();
		executedLines += file.getExecutedLinesCount();
		linesOfCode += file.getLinesOfCode();
	}
	
	public double getCoverage() {
		return (double)executedLines / (double)instrumentedLines;
	}

	public long getExecutedLinesCount() {
		return executedLines;
	}
	
	public long getInstrumentedLinesCount() {
		return instrumentedLines;
	}

	public long getLinesOfCode()
	{
		return linesOfCode;
	}
	
}
