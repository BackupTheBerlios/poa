/*
 * gcover
 * TODO
 * Created on 14.07.2003
 *
 */
package gcover;

/**
 * LineInfo
 * 
 * @author Tammo van Lessen
 * @version $Id: LineInfo.java,v 1.1 2003/07/15 18:06:32 vanto Exp $
 */
public class LineInfo {

	private boolean instrumented = false;
	private int execCount = 0;
	private String line = "";
	
	

	/**
	 * @return
	 */
	public int getExecCount() {
		return execCount;
	}

	/**
	 * @return
	 */
	public boolean isInstrumented() {
		return instrumented;
	}

	/**
	 * @return
	 */
	public String getSourceLine() {
		return line;
	}

	/**
	 * @param i
	 */
	public void setExecCount(int i) {
		execCount = i;
	}

	/**
	 * @param b
	 */
	public void setInstrumented(boolean b) {
		instrumented = b;
	}

	/**
	 * @param string
	 */
	public void setSourceLine(String string) {
		line = string;
	}

}
