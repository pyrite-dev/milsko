pipeline {
	agent {
		label "built-in"
	}
	stages {
		stage("Build document") {
			steps {
				sh("echo Testing testing...")
			}
			post {
				always {
					notifyDiscord()
				}
			}
		}
	}
}
