<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method = "text" indent = "no" /> 
<xsl:template match="/">
<xsl:for-each select="//release">
Version <xsl:value-of select="@version"/> (<xsl:value-of select="@date"/>)
<xsl:for-each select="action">
  * <xsl:value-of select="normalize-space(.)"/>

</xsl:for-each>
<xsl:value-of select="translate('&#10;',' ',' ')"/>
</xsl:for-each>
<xsl:value-of select="translate('&#10;',' ',' ')"/>
</xsl:template>

</xsl:stylesheet>