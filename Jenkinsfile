pipeline {
	agent {
		label "built-in"
	}
	stages {
		stage("Build document") {
			steps {
			}
			post {
				always {
					notifyDiscord()
				}
			}
		}
	}
}
