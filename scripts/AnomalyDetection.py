import numpy as np
import scipy.stats as stats

class AnomalyDetection:
    def __init__(self):
        a_th_ = None
        mu_ = None
        cov_ = None
        normalies_ = None
        anomalies_ = None
        anomaly_scores_ = None

    def setAnomalyThresholdHotteling(self, quantile):
        a_th = stats.chi2.isf(q=quantile, df=1)
        self.a_th_ = a_th

    def getAnomalyThreshold(self):
        return self.a_th_

    def setNormalDistributionParams(self, mu, cov):
        self.mu_ = mu
        self.cov_ = cov

    def estimateNormalDistributionParamsML(self, X):
        mu = np.mean(X, axis=0)
        cov = np.cov(X, rowvar=0, bias=True)
        self.setNormalDistributionParams(mu, cov)

    def calcAnomalyValueNormal(self, x):
        if len(x) == 1:
            a = (x - self.mu_)**2/self.cov_
        else:
            inv_cov = np.linalg.inv(self.cov_)
            a = np.dot(np.dot(x-self.mu_, inv_cov), x-self.mu_)
        return a

    def setAnomalyScores(self, anomaly_scores):
        self.anomaly_scores_ = anomaly_scores

    def getAnomalyScores(self):
        return self.anomaly_scores_

    def setAnomalyData(self, anomalies):
        self.anomalies_ = anomalies
        
    def getAnomalyData(self):
        return self.anomalies_

    def setNormalyData(self, normalies):
        self.normalies_ = normalies
        
    def getNormalyData(self):
        return self.normalies_

    def checkAnomalyData(self, anomaly_scores, X):
        for i, (a, val) in enumerate(zip(anomaly_scores, X)):
            if a >= self.a_th_:
                print "\x1b[31mRow "+str(i)+" is anomaly data.\x1b[0m"
                print "Value: "+str(val)
                print "Anomaly Score: "+str(a)
        if X[np.where(anomaly_scores >= self.a_th_)].shape[0] == 0:
            print "Data don't have anomaly datum."
        
    def normalDataAnomalyDetection(self, X, quantile, normaly_X=None):
        self.setAnomalyThresholdHotteling(quantile)
        if normaly_X is not None:
            self.estimateNormalDistributionParamsML(normaly_X)
        else:
            self.estimateNormalDistributionParamsML(X)
        anomaly_scores = np.array([self.calcAnomalyValueNormal(val) for val in X])
        self.checkAnomalyData(anomaly_scores, X)

        normalies = X[np.where(anomaly_scores < self.a_th_)[0]]
        anomalies = X[np.where(anomaly_scores >= self.a_th_)[0]]
        self.setAnomalyData(anomalies)
        self.setNormalyData(normalies)
        self.setAnomalyScores(anomaly_scores)
