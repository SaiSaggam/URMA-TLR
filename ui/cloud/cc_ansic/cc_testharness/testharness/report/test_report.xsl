<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
<html>
<head>
        <link href="test_report.css" media="screen" rel="stylesheet" type="text/css" />
</head>
<body>
    <xsl:for-each select="test_result">
        <xsl:for-each select="test_descriptor">
            <div class="test_descriptor_header_container">
                <span class="test_descriptor_header_text"><xsl:value-of select="name"/></span>
            </div>
            <div class="types_container">
            <xsl:for-each select="test_type">
                <div class="test_types_container">
                    <div class="test_type_header_container">
                        <span class="test_type_header_text"><xsl:value-of select="name"/></span>
                    </div>
                    <div class="groups_container">
                    <xsl:for-each select="groups/test_group">
                        <div class="test_group_container">
                            <div class="test_group_header_container">
                                <span class="test_group_header_text"><xsl:value-of select="name"/></span>
                            </div>
                            <div class="tests_container">
                                <xsl:for-each select="tests/test_name">
                                    <div class="test_name_container">
                                        <div class="test_name_header_container">
                                            <span class="test_name_header_text"><xsl:value-of select="name"/></span>
                                        </div>
                                        <div class="cases_container">
                                            <xsl:for-each select="cases/test_case">
                                                <div class="test_case_container">
                                                    <div class="test_case_header_container">
                                                        <span class="test_case_header_text"><xsl:value-of select="name"/></span>
                                                    </div>
                                                    <div class="test_case_description_container">
                                                        <pre class="test_case_description_text"><xsl:value-of select="description"/></pre>
                                                    </div>

                                                    <div class="test_case_result_container">
                                                        <xsl:choose>
                                                            <xsl:when test="result='True'">
                                                                <p class="test_case_result_text">PASSED</p>
                                                            </xsl:when>
                                                            <xsl:when test="result='False'">
                                                                <p class="test_case_result_text_red">FAILED</p>
                                                            </xsl:when>
                                                            <xsl:otherwise>
                                                                <!-- <p class="test_case_result_text">UNKNOWN</p> -->
                                                            </xsl:otherwise>
                                                        </xsl:choose>
                                                    </div>

                                                </div>
                                            </xsl:for-each>
                                        </div>
                                    </div>
                                </xsl:for-each>
                            </div>
                        </div>
                    </xsl:for-each>
                    </div>
                </div>
            </xsl:for-each>
            </div>
        </xsl:for-each>
    </xsl:for-each>
    <span class="footer_text">Test report generated automatically by the Cloud Connector Test Framework</span>
</body>
</html>
</xsl:template>

</xsl:stylesheet> 