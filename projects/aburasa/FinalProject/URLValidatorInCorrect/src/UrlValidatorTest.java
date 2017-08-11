/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import junit.framework.TestCase;





/**
 * Performs Validation Test for url validations.
 *
 * @version $Revision: 1128446 $ $Date: 2011-05-27 13:29:27 -0700 (Fri, 27 May 2011) $
 */
public class UrlValidatorTest extends TestCase {

   private boolean printStatus = false;
   private boolean printIndex = false;//print index that indicates current scheme,host,port,path, query test were using.
   UrlValidator urlVal;
   
   public UrlValidatorTest(String testName) {
      super(testName);
      urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
   }

   private String testURL(ResultPair urlPair, UrlValidator validator) {
	   String status = "";
	   String url = urlPair.item;
	   Boolean expectedValidity = urlPair.valid;
	   Boolean result = validator.isValid(url);
	   if(expectedValidity != result) {
		   status = "FAILED*: ";
	   } else {
		   status = "PASSED: ";
	   }
	   
	   return String.join("", status, url, ", expected: ", Boolean.toString(expectedValidity), ", actual: ", Boolean.toString(result));
   }
   
   public void testManualTest() {
	   System.out.println("---testManualTest---");
	   for(int i = 0; i < manualFullUrls.length; i++) {
		   System.out.println(testURL(manualFullUrls[i], urlVal));
	   }
	   System.out.println("-----------------------------------------------------------------");
   }
   
   
   public void testYourFirstPartition() {
	   System.out.println("---testYourFirstPartition---");
	   for(int i = 0; i < firstPartition.length; i++) {
		   System.out.println(testURL(firstPartition[i], urlVal));
	   }
	   System.out.println("-----------------------------------------------------------------");
   }
   
   public void testYourSecondPartition() {
	   System.out.println("---testYourSecondPartition---");
	   for(int i = 0; i < secondPartition.length; i++) {
		   System.out.println(testURL(secondPartition[i], urlVal));
	   }
	   System.out.println("-----------------------------------------------------------------");
   }
   
   private void debugAssertEquals (String item, Boolean expected, Boolean actual) {
	   if(expected != actual) {
		   System.out.println(String.join("", "***Error: ", item, ", expected=", Boolean.toString(expected), ", actual=", Boolean.toString(actual)));
	   }
   }
   
   public void testIsValid() {
	   String url;
	   Boolean expectedValidity;
	   int totalPermutations = 0;
	   int runningTotalDivisor = 1;
	   int[] cachedDivisors = new int[testUrlComponents.length];
	   
	   for(int i = 0; i < testUrlComponents.length; i++) {
		    if(testUrlComponents[i].length > 0) {
			   cachedDivisors[i] = runningTotalDivisor;
			   runningTotalDivisor *= testUrlComponents[i].length;
			   if(totalPermutations == 0) {
				   totalPermutations = testUrlComponents[i].length;
			   } else {
				   totalPermutations *= testUrlComponents[i].length;
			   }
		   } else {
			   cachedDivisors[i] = 0;
		   }
		   //System.out.println(String.join("", "divisors[", Integer.toString(i), "] = ", Integer.toString(divisors[i])));
	   }
	   
	   //System.out.println("Total permutations: " + Integer.toString(totalPermutations));
	   System.out.println("---testIsValid---");
	   for(int permutation = 0; permutation < totalPermutations; permutation++) {
		   url = "";
		   expectedValidity = true;
		   for(int i = 0; i < testUrlComponents.length; i++) {
			   if(cachedDivisors[i] > 0) {
				   int index = Math.floorDiv(permutation, cachedDivisors[i]) % testUrlComponents[i].length;
				   expectedValidity = expectedValidity && testUrlComponents[i][index].valid;
				   url = String.join("", url, testUrlComponents[i][index].item);
			   }
	       }
		   debugAssertEquals(url, expectedValidity, urlVal.isValid(url));
		   //System.out.println(testURL(new ResultPair(url, expectedValidity), urlVal));
   	   }
	   System.out.println("-----------------------------------------------------------------");
	   
   }
   
   public void testAnyOtherUnitTest() {
	   
   }
   /**
    * Create set of tests by taking the testUrlXXX arrays and
    * running through all possible permutations of their combinations.
    *
    * @param testObjects Used to create a url.
    */
   
   ResultPair[] testUrlSchemes = {
       new ResultPair("http://", true),
       new ResultPair("https://", true),
	   new ResultPair("", true),
	   new ResultPair("http:/", false),
	   new ResultPair("http:", false)
   };
   
   ResultPair[] testUrlAuthorities = {
	   new ResultPair("google.com", true),
	   new ResultPair("amazon.com", true),
	   new ResultPair("", false)
   };
   
   ResultPair[] testUrlPorts = {
	   new ResultPair(":-1", false),
	   new ResultPair("", true),
	   new ResultPair(":80", true),
	   new ResultPair(":80b", false)
   };
   
   ResultPair[] testUrlPaths = {
	   new ResultPair("", true),
   };
   
   ResultPair[] testUrlQueries = {
		   new ResultPair("?action=view", true),
		   new ResultPair("", true)
   };
   
   ResultPair[][] testUrlComponents = {
      testUrlSchemes,
      testUrlAuthorities,
      testUrlPorts,
      testUrlPaths,
      testUrlQueries
   };
   
   //Any URLs listed here will be tested manually.
   ResultPair[] manualFullUrls = {
      new ResultPair("http://google.com", true),
      new ResultPair("https://google.com", true),
      new ResultPair("http://///////google.com", false),
      new ResultPair("http://amazon.com", true),
      new ResultPair("http://amazon.com:80", true),
      new ResultPair("http://google.com/search?banana", false),
      new ResultPair("http://google.com/search?q=banana", true)
   };
   
   //URLs that do not have path or query
   ResultPair[] firstPartition = {
      new ResultPair("http://firstPartition.balh", false)
   };
   
   //URLs that have path or query
   ResultPair[] secondPartition = {
	  new ResultPair("http://secondPartition.balh", false),
	  new ResultPair("http://google.com/test1?action=view", true)
   };

}
