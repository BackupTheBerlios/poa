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
 * @version $Id: Project.java,v 1.4 2004/01/14 15:33:15 squig Exp $
 */
public class Project {

	private Map fileByName = new Hashtable();
	private long instrumentedLines = 0;
	private long executedLines = 0;
	private long linesOfCode = 0;

	public void addFile(FileInfo file) {
		fileByName.put(file.getName(), file);

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

	public FileInfo getFile(String filename)
	{
		return (FileInfo)fileByName.get(filename);
	}

	public int getFileCount()
	{
		return fileByName.size();
	}

	public FileInfo[] getFiles() {
		FileInfo[] array
			= (FileInfo[])fileByName.values().toArray(new FileInfo[0]);
		Arrays.sort(array);
		return array;
	}
	
	public long getInstrumentedLinesCount() {
		return instrumentedLines;
	}

	public long getLinesOfCode()
	{
		return linesOfCode;
	}
	
}
