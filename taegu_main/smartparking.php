<!--
	Full Motion by TEMPLATED
	templated.co @templatedco
	Released for free under the Creative Commons Attribution 3.0 license (templated.co/license)
-->
<html>
	<head>
		<title>스마트 주차장관리 시스템</title>
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1" />
		<link rel="stylesheet" href="assets/css/main.css" />
	</head>
	<body id="top">
			<!-- Banner -->
			<!--
				To use a video as your background, set data-video to the name of your video without
				its extension (eg. images/banner). Your video must be available in both .mp4 and .webm
				formats to work correctly.
			-->
				<section id="banner" data-video="images/banner">
					<div class="inner">
						<header>
							<h1>Smart Parking Management System</h1>
							<p>부산대학교 스마트 주차장 관리 시스템입니다.</p><br/>
							현재 저희가 운영하는 시스템은 부산대학교 항공관, 자연동 주차장 2곳을 지원합니다.</p>
						</header>
						<a href="#main" class="more">Learn More</a>
					</div>
				</section>
			<!-- Main -->
				<div id="main">
					<div class="inner">

					<!-- Boxes -->
						<div class="thumbnails">
							<div class="box">
								<viedo src="video/back_side_01.avi" class="image fit">
									<img src="images/pic01.jpg" alt="" /></a>
								 <div class="inner">
									<h3>항공관 주차장</h3>
									<p>총 주차가능 공간 : <?php
									  $db = mysqli_connect("localhost:3307", "root", "rkdxorn", "test");
									  /*if($db){
									      echo "connect : 성공<br>";
									  }
									  else{
									      echo "disconnect : 실패<br>";
									  }*/
									  mysqli_set_charset($db,"utf8");
									  $sql="SELECT * FROM PARKING";                         //TEST 테이블의 레코드를 모두 뽑아오기

									  $result = mysqli_query($db, $sql);
									  $count = mysqli_num_rows($result);
									  echo $count
										?>
									</p>현재 주차가능 공간 :   <?php
									    $db = mysqli_connect("localhost:3307", "root", "rkdxorn", "test");
									    /*if($db){
									        echo "connect : 성공<br>";
									    }
									    else{
									        echo "disconnect : 실패<br>";
									    }*/
									    mysqli_set_charset($db,"utf8");
									    $sql="SELECT * FROM PARKING where level=0";                         //TEST 테이블의 레코드를 모두 뽑아오기

									    $result = mysqli_query($db, $sql);
									    $count = mysqli_num_rows($result);
									    echo $count
									    ?>
										<p></p>
									<a href="https://youtu.be/s6zR2T9vn2c" class="button fit" data-poptrox="youtube,800x400">상세보기</a>
								</div>
							</div>
							<div class="box">
								<a href="https://youtu.be/s6zR2T9vn2c" class="image fit"><img src="images/pic03.jpg" alt="" /></a>
								<div class="inner">
									<h3>자연동 주차장</h3>
									<p>총 주차가능 공간 : <?php
									  $db = mysqli_connect("localhost:3307", "root", "rkdxorn", "test");
									  /*if($db){
									      echo "connect : 성공<br>";
									  }
									  else{
									      echo "disconnect : 실패<br>";
									  }*/
									  mysqli_set_charset($db,"utf8");
									  $sql="SELECT * FROM PARKING2";                         //TEST 테이블의 레코드를 모두 뽑아오기

									  $result = mysqli_query($db, $sql);
									  $count = mysqli_num_rows($result);
									  echo $count ?> </p>
									현재 주차 가능 공간 : <?php
									    $db = mysqli_connect("localhost:3307", "root", "rkdxorn", "test");
									    /*if($db){
									        echo "connect : 성공<br>";
									    }
									    else{
									        echo "disconnect : 실패<br>";
									    }*/
									    mysqli_set_charset($db,"utf8");
									    $sql="SELECT * FROM PARKING2 where level=0";                         //TEST 테이블의 레코드를 모두 뽑아오기

									    $result = mysqli_query($db, $sql);
									    $count = mysqli_num_rows($result);
									    echo $count
									    ?><p></p>
									<a href="https://youtu.be/s6zR2T9vn2c" class="button style2 fit" data-poptrox="youtube,800x400">상세보기</a>
								</div>
							</div>

							<div class="box">
								<a href="https://youtu.be/s6zR2T9vn2c" class="image fit"><img src="images/pic02.jpg" alt="" /></a>
								<div class="inner">
									<h3>Contact</h3>
									<p>부산대학교 정보컴퓨터공학부 어쩌조</p>
									강태구, 임택기, 손석현<p></p>
									<a href="#" class="button style3 fit" data-poptrox="youtube,800x400">상세보기</a>
								</div>
							</div>
						</div>

					</div>
				</div>

			<!-- Footer -->
				<footer id="footer">
					<div class="inner">
						<h2>부산대학교 정보컴퓨터공학부 졸업과제</h2>
						<p>어쩌조</p>
						<p>강태구 손석현 임택기</p>

						<ul class="icons">
							<li><a href="#" class="icon fa-twitter"><span class="label">Twitter</span></a></li>
							<li><a href="#" class="icon fa-facebook"><span class="label">Facebook</span></a></li>
							<li><a href="#" class="icon fa-instagram"><span class="label">Instagram</span></a></li>
							<li><a href="#" class="icon fa-envelope"><span class="label">Email</span></a></li>
						</ul>
						<p class="copyright">&copy; 어쩌조. Design: <a href="#">taeguKang</a>. Images: <a href="#">sonseokHyun</a>. Videos: <a href="#">taekgiLim</a>.</p>
					</div>
				</footer>

		<!-- Scripts -->
			<script src="assets/js/jquery.min.js"></script>
			<script src="assets/js/jquery.scrolly.min.js"></script>
			<script src="assets/js/jquery.poptrox.min.js"></script>
			<script src="assets/js/skel.min.js"></script>
			<script src="assets/js/util.js"></script>
			<script src="assets/js/main.js"></script>
	</body>
</html>
