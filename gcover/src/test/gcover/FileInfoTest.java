/*
 * gcover
 * TODO
 * Created on 14.07.2003
 *
 */
package gcover;

import junit.framework.TestCase;

/**
 * FileInfoTest
 * 
 * @author Tammo van Lessen
 * @version $Id: FileInfoTest.java,v 1.2 2004/01/12 13:45:59 squig Exp $
 */
public class FileInfoTest extends TestCase {

	private FileInfo fileInfo;

	/**
	 * Constructor for FileInfoTest.
	 * @param arg0
	 */
	public FileInfoTest(String arg0) {
		super(arg0);
	}

	public void setUp() {
		fileInfo = new FileInfo("package", "filename");
		LineInfo li = new LineInfo();
		li.setExecCount(1);
		li.setInstrumented(true);
		li.setSourceLine("abc");
		fileInfo.addLine(li);
	}

	public void tearDown() {
		fileInfo = null;
	}

	public void testFileInfo() {
	}

	public void testGetLines() {

	}

	public void testGetName() {
		assertEquals(fileInfo.getPackageName(), "package");
		assertEquals(fileInfo.getName(), "filename");
	}

	public void testSetLines() {
	}

}
