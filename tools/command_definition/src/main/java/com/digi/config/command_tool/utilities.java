package com.digi.config.command_tool;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class utilities {

	public final static int INT32_MIN_VALUE = -2147483648;
	public final static int INT32_MAX_VALUE = 2147483647;
	public final static long UINT32_MAX_VALUE = 4294967295L;

	public static boolean isInteger(String str) {
		try {
			Long.parseLong(str);
			return true;
		} catch (Exception e) {
			return false;
		}
	}

//	private static final String IPADDRESS_PATTERN = "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$";

	private static final Pattern ipv4Pattern = Pattern
			.compile("(([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.){3}([01]?\\d\\d?|2[0-4]\\d|25[0-5])");
//	.compile(IPADDRESS_PATTERN);

	// private static final Pattern ipv6Pattern =
	// Pattern.compile("([0-9a-fA-F]{1,4}:){7}([0-9a-fA-F]){1,4}",
	// Pattern.CASE_INSENSITIVE);

	public static boolean checkIpAddress(String s) {
		if (s == null)
			return false;
		else {
			Matcher ipv4 = ipv4Pattern.matcher(s);
			if (ipv4.matches())
				return true;
			else
				return false;

			// Matcher ipv6 = ipv6Pattern.matcher(s);
			// return ipv6.matches();
		}
	}

	private static final Pattern macAddressPatter = Pattern
			.compile("(?:[A-Fa-f0-9]{2}[:-]){5}(?:[A-Fa-f0-9]{2})");

	public static boolean checkMacAddress(String s) {
		if (s == null)
			return false;
		else {
			Matcher mac = macAddressPatter.matcher(s);
			if (mac.matches())
				return true;
			else
				return false;

		}
	}

}
