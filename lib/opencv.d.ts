declare module 'opencv' {
    import 'node';
    import { Stream } from 'stream';

    export type Point2F = {
        x: number;
        y: number;
    }

    export type Point3F = {
        x: number;
        y: number;
        z: number;
    }

    export type PointLike = {
        x: number;
        y: number;
    }
    
    export type SizeLike = {
        width: number;
        height: number;
    }
    
    export type RectLike = Point2F & SizeLike;

    export type ScalarLike = [number, number, number, number];

    export type ArraySize = [number, number];
    export type ArrayPoint = [number, number];
    export type ArrayScalar = [number, number, number];
    export type ArrayColor = [number, number, number];
    export type ArrayRect = [number, number, number, number];

    export type HoughCircle = [number, number, number];
    export type HoughLine = [number, number, number, number];

    export type ContourHierarchy = [number, number, number, number];
    export type SerializedContours = { contours: Point2F[][], hierarchy: ContourHierarchy[] };

    export type MatrixType = number;
    export type BorderType = number;
    export type InterpolationMode = number;
    export type NormalizationType = number;
    export type ContourRetrievalMode = number;
    export type MorphShape = number;
    export type StereoPreset = number;
    /**
     * -1 = FILLED
     * 4 = LINE_4
     * 8 = LINE_8
     * 16 = LINE_AA
     */
    export type LineType = -1 | 4 | 8 | 16;
    /**
     * 0 = FONT_HERSHEY_SIMPLEX
     * 1 = FONT_HERSHEY_PLAIN
     * 2 = FONT_HERSHEY_DUPLEX
     * 3 = FONT_HERSHEY_COMPLEX
     * 4 = FONT_HERSHEY_TRIPLEX
     * 5 = FONT_HERSHEY_COMPLEX_SMALL
     * 6 = FONT_HERSHEY_SCRIPT_SIMPLEX
     * 7 = FONT_HERSHEY_SCRIPT_COMPLEX
     * 16 = FONT_ITALIC
     */
    export type HershyFont = 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 16;
    /**
     * 0 = ADAPTIVE_THRESH_MEAN_C
     * 1 = ADAPTIVE_THRESH_GAUSSIAN_C
     */
    export type AdaptiveThresholdMethod = 0 | 1;
    /**
     * 0 = THRESH_BINARY
     * 1 = THRESH_BINARY_INV
     * 2 = THRESH_TRUNC
     * 3 = THRESH_TOZERO
     * 4 = THRESH_TOZERO_INV
     * 7 = THRESH_MASK
     * 8 = THRESH_OTSU
     * 16 = THRESH_TRIANGLE
     */
    export type ThresholdType = 0 | 1 | 2 | 3 | 4 | 7 | 8 | 16;
    export type TemplateMatchMode = number;

    export type FaceRecognizerTrainingData = [number, Matrix][];

    export type CascadeClassifierOptions = {
        scale?: number;
        neighbors?: number;
        min?: ArraySize;
    };

    export type MatrixToBufferOptions = {
        ext: string;
        jpegQuality: number;
        pngCompression: number;
    };

    export type MatrixEllipseOptions = {
        center: Point2F;
        axes: SizeLike;
        thickness: number;
        angle: number;
        startAngle: number;
        endAngle: number;
        lineType: LineType;
        shift: number;
        color: ArrayColor;
    };
    
    export type Moments = {
        // spatial moments
        m00: number; 
        m10: number; 
        m01: number; 
        m20: number; 
        m11: number; 
        m02: number; 
        m30: number; 
        m21: number; 
        m12: number; 
        m03: number;
        // central moments
        mu20: number; 
        mu11: number; 
        mu02: number; 
        mu30: number; 
        mu21: number; 
        mu12: number; 
        mu03: number;
        // central normalized moments
        nu20: number; 
        nu11: number; 
        nu02: number; 
        nu30: number; 
        nu21: number; 
        nu12: number; 
        nu03: number;
    }

    export const version: string;

    export function readImage(buffer: Buffer): Promise<Matrix>;
    export function readImage(filename: string): Promise<Matrix>;
    export function readImage(buffer: Buffer, callback: (err: Error, image: Matrix) => void): void;
    export function readImage(filename: string, callback: (err: Error, image: Matrix) => void): void;

    export class Point {
        x: number;
        y: number;
        constructor();
        constructor(point: PointLike);
        constructor(x: number, y: number);
        dot(p2: Point): number;
    }

    export class Size {
        width: number;
        height: number;
        constructor();
        constructor(size: SizeLike);
        constructor(width: number, height: number);
        area(): number;
    }

    export class Rect {
        x: number;
        y: number;
        width: number;
        height: number;
        constructor();
        constructor(rect: RectLike);
        constructor(origin: PointLike, size: SizeLike);
        constructor(point1: PointLike, point2: PointLike);
        constructor(x: number, y: number, width: number, height: number);
        tl(): Point;
        br(): Point;
        size(): Size;
        area(): number;
        contains(rect: RectLike): boolean;
    }

    export class Scalar {
        [index: number]: number;
        constructor();
        constructor(scalar: ScalarLike);
        constructor(v0: number, v1: number, v2: number, v3: number);
        mul(scalar: Scalar, scale?: number): Scalar;
        conj(): Scalar;
        isReal(): boolean;
    }

    export namespace Matrix {
        export function Zeros(width: number, height: number, type?: MatrixType): Matrix;
        export function Ones(width: number, height: number, type?: MatrixType): Matrix;
        export function Eye(width: number, height: number, type?: MatrixType): Matrix;
        export function getRotationMatrix2D(angle: number, x: number, y: number, scale?: number): Matrix;
    }

    export class Matrix {
        constructor();
        constructor(rows: number, cols: number, type?: MatrixType, scalar?: ArrayScalar);
        constructor(x: number, y: number, w: number, h: number, src: Matrix);
        setTo(value: number | Matrix, mask?: Matrix): void;
        row(): number;
        col(): number;
        pixelRow(y: number): number[];
        pixelCol(x: number): number[];
        empty(): boolean;
        get(x: number, y: number): number;
        set(x: number, y: number, value: number, channel?: number): void;
        put(buf: Buffer): void;
        brightness(diff: number): void;
        brightness(alpha: number, beta: number): void;
        normalize(min: number, max: number, type?: NormalizationType, dtype?: number): void;
        norm(src2: Matrix): number;
        norm(type: NormalizationType): number;
        norm(src2: Matrix, type: NormalizationType): number;
        norm(type: NormalizationType, mask: Matrix): number;
        norm(src2: Matrix, type: NormalizationType, mask: Matrix): number;
        getData(): Buffer;
        pixel(x: number, y: number): ArrayColor | number;
        pixel(x: number, y: number, color?: ArrayColor | [number]): ArrayColor | [number];
        width(): number;
        height(): number;
        size(): Size;
        clone(): Matrix;
        crop(x: number, y: number, width: number, height: number): Matrix;
        toBuffer(opt?: MatrixToBufferOptions): Buffer;
        toBuffer(callback: (err: Error, buf: Buffer) => void, opt?: MatrixToBufferOptions): void;
        toBufferAsync(callback: (err: Error, buf: Buffer) => void, opt?: MatrixToBufferOptions): void;
        ellipse(opts: MatrixEllipseOptions): void;
        ellipse(x: number, y: number, width: number, height: number, color?: ArrayColor, thickness?: number): void;
        rectangle(topLeft: ArraySize, widthHeight: ArrayPoint, color?: ArrayColor, thickness?: number): void;
        line(pt1: Point2F, p2: Point2F, color?: ArrayColor, thickness?: number): void;
        fillPoly(polygons: ArrayPoint[][], color?: ArrayColor): void;
        save(filename: string): number;
        save(filename: string, callback: (err: Error, result: number) => void): void;
        saveAsync(filename: string, callback: (err: Error, result: number) => void): void;
        resize(size: SizeLike, fx?: number, fy?: number, interpolation?: InterpolationMode): Matrix;
        rotate(angle: number, x: number, y: number): void;
        warpAffine(rotation: Matrix, dstRows: number, dstCols: number): void;
        copyTo(dst: Matrix, dstX: number, dstY: number): void;
        convertTo(dst: Matrix, type: MatrixType, scale?: number, delta?: number): void;
        pyrDown(): void;
        pyrUp(): void;
        channels(): number;
        convertGrayscale(): void;
        convertHSVscale(): void;
        gaussianBlur(ksize: ArraySize): void;
        medianBlur(ksize: number): void;
        bilateralFilter(): void;
        bilateralFilter(diameter: number, maxSigmaColor: number, sigmaSpace: number, borderType?: BorderType): void;
        sobel(ddepth: number, xorder: number, yorder: number, ksize?: number, scale?: number, delta?: number, borderType?: BorderType): Matrix;
        copy(): Matrix;
        flip(flipCode: 0 | 1 | -1): Matrix;
        roi(rect: RectLike): Matrix;
        roi(point: PointLike, size: SizeLike): Matrix;
        roi(point1: PointLike, point2: PointLike): Matrix;
        roi(x: number, y: number, width: number, height: number): Matrix;
        ptr(line: number): Buffer;
        absDiff(src1: Matrix, src2: Matrix): void;
        addWeighted(src1: Matrix, weight1: number, src2: Matrix, weight2: number);
        bitwiseXor(src1: Matrix, src2: Matrix, mask?: Matrix): void;
        bitwiseNot(src1: Matrix, src2: Matrix, mask?: Matrix): void;
        bitwiseAnd(src1: Matrix, src2: Matrix, mask?: Matrix): void;
        countNonZero(): number;
        moments(): Moments;
        canny(low: number, high: number): void;
        dilate(iterations: number, kernel?: Matrix): void;
        erode(iterations: number, kernel?: Matrix): void;
        findContours(mode?: number, chain?: number): Contours;
        drawContour(contours: Contours, pos: number, color?: ArrayColor, thickness?: number);
        drawAllContours(contours: Contours, color?: ArrayColor, thickness?: number);
        goodFeaturesToTrack(): ArrayPoint[];
        houghLinesP(rho?: number, theta?: number, threshold?: number, minLineLength?: number, maxLineGap?: number): HoughLine[];
        houghCircles(dp?: number, minDist?: number, higherThreshold?: number, accumulatorThreshold?: number, minRadius?: number, maxRadius?: number): HoughCircle[];
        inRange(low: ArrayColor, high: ArrayColor): void;
        adjustROI(dtop: number, dbottom: number, dleft: number, dright: number): number;
        locateROI(): ArrayRect;
        threshold(threshold: number, maxVal: number, type?: "Binary" | "Binary Inverted" | "Threshold Truncated" | "Threshold to Zero" | "Threshold to Zero Inverted", algorithm?: "Simple" | "Otsu"): Matrix;
        adaptiveThreshold(maxVal: number, adaptiveMethod: AdaptiveThresholdMethod, thresholdType: ThresholdType, blockSize: number, C: number);
        meanStdDev(): { mean: Matrix, stddev: Matrix };
        cvtColor(code: "CV_BGR2GRAY" | "CV_GRAY2BGR" | "CV_BGR2XYZ" | "CV_XYZ2BGR" | "CV_BGR2YCrCb" | "CV_YCrCb2BGR" | "CV_BGR2HSV" | "CV_HSV2BGR" | "CV_BGR2HLS" | "CV_HLS2BGR" | "CV_BGR2Lab" | "CV_Lab2BGR" | "CV_BGR2Luv" | "CV_Luv2BGR" | "CV_BayerBG2BGR" | "CV_BayerGB2BGR" | "CV_BayerRG2BGR" | "CV_BayerGR2BGR" | "CV_BGR2RGB"): void;
        split(): Matrix[];
        merge(channels: Matrix[]): void;
        equalizeHist(): void;
        floodFill(opt: { seedPoint: ArrayPoint, newColor: ArrayColor, rect: [ArrayPoint, ArraySize], loDiff: ArrayColor, upDiff: ArrayColor }): number;
        matchTemplate(templ: Matrix, method: TemplateMatchMode, mask?: Matrix): Matrix;
        templateMatches(minProbability?: number, maxProbability?: number, limit?: number, ascending?: boolean, minXDistance?: number, minYDistance?: number): Array<Point2F & { probability: number }>;
        minMaxLoc(): { minVal: number, maxVal: number, minLoc: Point, maxLoc: Point };
        pushBack(mat: Matrix);
        putText(text: string, x: number, y: number, font?: "HERSEY_SIMPLEX" | "HERSEY_PLAIN" | "HERSEY_DUPLEX" | "HERSEY_COMPLEX" | "HERSEY_TRIPLEX" | "HERSEY_COMPLEX_SMALL" | "HERSEY_SCRIPT_SIMPLEX" | "HERSEY_SCRIPT_COMPLEX" | "HERSEY_SCRIPT_SIMPLEX", color?: ArrayColor, scale?: number, thickness?: number);
        getPerspectiveTransform(srcCorners: Point2F[], targetCorners: Point2F[]): Matrix;
        warpPerspective(M: Matrix, width: number, height: number, color?: ArrayColor): void;
        copyWithMask(dst: Matrix, mask: Matrix): void;
        setWithMask(value: ArrayColor, mask: Matrix): void;
        meanWithMask(mask: Matrix): Scalar;
        mean(): Scalar;
        shift(deltaX: number, deltaY: number): void;
        reshape(channels: number, rows?: number): Matrix;
        release(): void;
        subtract(src2: Matrix): void;

        detectObject(classifier: string, opts: CascadeClassifierOptions, callback: (err: Error, objects: RectLike[]) => void);
    }

    export class CascadeClassifier {
        constructor(filename: string);
        detectMultiScale(image: Matrix, callback: (err: Error, objects: RectLike[]) => void, scale?: number, neighbors?: number, minWidth?: number, minHeight?: number);
    }

    export class VideoCapture {
        constructor(device: number);
        constructor(filename: string);
        read(callback: (err: Error, image: Matrix) => void): void;
        setWidth(width: number): void;
        setHeight(height: number): void;
        setPosition(position: number): void;
        getFrameAt(position: number): void;
        getFrameCount(): number;
        release(): void;
        ReadSync(): Matrix;
        grab(callback: (err: Error, image: Matrix) => void): void;
        retrieve(callback: (err: Error, image: Matrix) => void, channel: number): void;
        toStream(): VideoStream;
    }

    export class Contours {
        point(pos: number, index: number): Point2F;
        points(pos: number): Point2F[];
        size(): number;
        cornerCount(pos: number): number;
        area(pos: number): number;
        arcLength(pos: number, isClosed: boolean): number;
        approxPolyDP(pos: number, epsilon: number, isClosed: boolean): void;
        convexHull(pos: number, clockwise: boolean): void;
        boundingRect(pos: number): RectLike;
        minAreaRect(pos: number): { angle: number, size: SizeLike, /*center: Point2F, */points: Point2F[] };
        fitEllipse(pos: number): { angle: number, size: SizeLike, center: Point2F };
        isConvex(pos: number): boolean;
        moments(pos: number): { m00: number, m10: number, m01: number, m11: number };
        hierarchy(pos: number): ContourHierarchy;
        serialize(): SerializedContours;
        deserialize(serialized: SerializedContours): void;
    }

    export class TrackedObject {
        constructor(image: Matrix, rect: ArrayRect, opts?: { channel: "h" | "hue" | "s" | "saturation" | "v" | "value" });
        track(image: Matrix): ArrayRect;
    }

    export class NamedWindow {
        constructor(name: string);
        show(image: Matrix): this;
        destroy(): this;
        blockingWaitKey(time?: number): number;
    }

    export namespace Constants {
        const CV_8U: MatrixType;
        const CV_8S: MatrixType;
        const CV_16U: MatrixType;
        const CV_16S: MatrixType;
        const CV_32S: MatrixType;
        const CV_32F: MatrixType;
        const CV_64F: MatrixType;
        const CV_USRTYPE1: MatrixType;

        const CV_8UC1: MatrixType;
        const CV_8UC2: MatrixType;
        const CV_8UC3: MatrixType;
        const CV_8UC4: MatrixType;

        const CV_8SC1: MatrixType;
        const CV_8SC2: MatrixType;
        const CV_8SC3: MatrixType;
        const CV_8SC4: MatrixType;

        const CV_16UC1: MatrixType;
        const CV_16UC2: MatrixType;
        const CV_16UC3: MatrixType;
        const CV_16UC4: MatrixType;

        const CV_16SC1: MatrixType;
        const CV_16SC2: MatrixType;
        const CV_16SC3: MatrixType;
        const CV_16SC4: MatrixType;

        const CV_32SC1: MatrixType;
        const CV_32SC2: MatrixType;
        const CV_32SC3: MatrixType;
        const CV_32SC4: MatrixType;

        const CV_32FC1: MatrixType;
        const CV_32FC2: MatrixType;
        const CV_32FC3: MatrixType;
        const CV_32FC4: MatrixType;

        const CV_64FC1: MatrixType;
        const CV_64FC2: MatrixType;
        const CV_64FC3: MatrixType;
        const CV_64FC4: MatrixType;

        const BORDER_DEFAULT: BorderType;
        const BORDER_REPLICATE: BorderType;
        const BORDER_REFLECT: BorderType;
        const BORDER_REFLECT_101: BorderType;
        const BORDER_WRAP: BorderType;
        const BORDER_CONSTANT: BorderType;

        const INTER_NEAREST: InterpolationMode;
        const INTER_LINEAR: InterpolationMode;
        const INTER_AREA: InterpolationMode;
        const INTER_CUBIC: InterpolationMode;
        const INTER_LANCZOS4: InterpolationMode;

        const NORM_MINMAX: NormalizationType;
        const NORM_INF: NormalizationType;
        const NORM_L1: NormalizationType;
        const NORM_L2: NormalizationType;
        const NORM_L2SQR: NormalizationType;
        const NORM_HAMMING: NormalizationType;
        const NORM_HAMMING2: NormalizationType;
        const NORM_RELATIVE: NormalizationType;
        const NORM_TYPE_MASK: NormalizationType;

        const RETR_EXTERNAL: ContourRetrievalMode;
        const RETR_LIST: ContourRetrievalMode;
        const RETR_CCOMP: ContourRetrievalMode;
        const RETR_TREE: ContourRetrievalMode;
        
        const TM_SQDIFF: TemplateMatchMode;
        const TM_SQDIFF_NORMED: TemplateMatchMode;
        const TM_CCORR: TemplateMatchMode;
        const TM_CCORR_NORMED: TemplateMatchMode;
        const TM_CCOEFF: TemplateMatchMode;
        const TM_CCOEFF_NORMED: TemplateMatchMode;
    }

    export namespace calib3d {
        export function findChessboardCorners(image: Matrix, size: ArraySize): void;
        export function drawChessboardCorners(image: Matrix, size: ArraySize, corners: Point2F[], patternWasFound: boolean): void;
        export function calibrateCamera(objectPoints: Point3F[][], imagePoints: Point2F[][], imageSize: ArraySize): { reprojectionError: number, K: Matrix, distortion: Matrix };
        export function solvePnP(objectPoints: Point3F[][], imagePoints: Point2F[][], K: Matrix, distortion: Matrix): { rvec: Matrix, tvec: Matrix };
        export function getOptimalNewCameraMatrix(K: Matrix, distortion: Matrix, imageSize: ArraySize, alpha: number, newImageSize: ArraySize): Matrix;
        export function stereoCalibrate(objectPoints: Point3F[][], imagePoints1: Point2F[][], imagePoints2: Point2F[][], imageSize: ArraySize, K1: Matrix, d1: Matrix, K2: Matrix, d2: Matrix): { K1: Matrix, distortion1: Matrix, K2: Matrix, distortion2: Matrix, R: Matrix, t: Matrix, E: Matrix, F: Matrix };
        export function stereoRectify(K1: Matrix, d1: Matrix, K2: Matrix, d2: Matrix, imageSize: ArraySize, R: Matrix, t: Matrix): { R1: Matrix, R2: Matrix, P1: Matrix, P2: Matrix, Q: Matrix };
        export function computeCorrespondEpilines(points: Point2F[], whichImage: 1 | 2, F: Matrix): { a: number, b: number, c: number }[];
        export function reprojectImageTo3d(disparity: Matrix, Q: Matrix): Matrix;
    }

    export namespace imgproc {
        export function undistort(image: Matrix, K: Matrix, distortion: Matrix): Matrix;
        export function initUndistortRectifyMap(K: Matrix, distortion: Matrix, R: Matrix, newK: Matrix, imageSize: ArraySize, m1type: MatrixType): { map1: Matrix, map2: Matrix };
        export function remap(image: Matrix, map1: Matrix, map2: Matrix, interpolation: InterpolationMode): Matrix;
        export function getStructuringElement(shape: MorphShape, ksize: ArraySize): Matrix;
    }

    abstract class Stereo {
        compute(left: Matrix, right: Matrix, type?: MatrixType): Matrix;
    }

    export namespace StereoBM {
        export const BASIC_PRESET: StereoPreset;
        export const FISH_EYE_PRESET: StereoPreset;
        export const NARROW_PRESET: StereoPreset;
    }

    export class StereoBM extends Stereo {
        constructor(preset?: StereoPreset, ndisparities?: number, SADWindowSize?: number);
    }

    export class StereoSGBM extends Stereo {
        constructor(minDisparity: number, ndisparities: number, SADWindowSize: number, p1?: number, p2?: number, disp12MaxDiff?: number, preFilterCap?: number, uniquenessRatio?: number, speckleWindowSize?: number, speckleRange?: number, fullDP?: boolean);
    }

    export class StereoGC extends Stereo {
        constructor(ndisparities?: number, maxIters?: number);
    }

    export namespace BackgroundSubtractor {
        export function createMOG(): BackgroundSubtractor;
    }

    export class BackgroundSubtractor {
        applyMOG(image: Matrix, callback: (err: Error, foregroundMask: Matrix) => void): void;
        applyMOG(image: Buffer, callback: (err: Error, foregroundMask: Matrix) => void): void;
    }

    export function ImageSimilarity(image1: Matrix, image2: Matrix, callback: (err: Error, dissimilarity: number) => void): void;

    export namespace LDA {
        export function subspaceProject(W: Matrix, mean: Matrix, src: Matrix): Matrix;
        export function subspaceReconstruct(W: Matrix, mean: Matrix, src: Matrix): Matrix;
    }

    export class LDA {}

    export namespace FaceRecognizer {
        export function createLBPHFaceRecognizer(radius?: number, neighbors?: number, gridX?: number, gridY?: number, threshold?: number): FaceRecognizer;
        export function createEigenFaceRecognizer(components?: number, threshold?: number): FaceRecognizer;
        export function createFisherFaceRecognizer(components?: number, threshold?: number): FaceRecognizer;
    }

    export class FaceRecognizer {
        trainSync(data: FaceRecognizerTrainingData): void;
        train(data: FaceRecognizerTrainingData, callback: (err: Error) => void): void;
        updateSync(data: FaceRecognizerTrainingData): void;
        predictSync(image: Matrix | string): { id: number, confidence: number };
        predict(image: Matrix | string, callback: (err: Error, result: { id: number, confidence: number }) => void): void;
        saveSync(filename: string): void;
        loadSync(filename: string): void;

        getMat(key: "mean" | "eigenvectors" | "eigenvalues"): Matrix;
    }

    export class ImageStream extends Stream {
        write(buf: Buffer): void;

        on(event: "error", listener: (err: Error) => void): this;
        on(event: "data", listener: (image: Matrix) => void): this;
    }

    export class ImageDataStream extends Stream {
        writable: boolean;
        write(buf: Buffer): void;
        end(buf?: Buffer): void;

        on(event: "error", listener: (err: Error) => void): this;
        on(event: "load", listener: (image: Matrix) => void): this;
    }

    export class ObjectDetectionStream extends Stream {
        readable: boolean;
        writable: boolean;
        constructor(cascadeFilename: string, opts: CascadeClassifierOptions);
        write(image: Matrix);

        on(event: "error", listener: (err: Error) => void): this;
        on(event: "data", listener: (objects: RectLike[], image: Matrix) => void): this;
    }

    export class VideoStream extends Stream {
        readable: boolean;
        constructor(src: VideoCapture);
        constructor(device: number);
        constructor(filename: string);
        read(): void;
        pause(): void;
        resume(): void;

        on(event: "error", listener: (err: Error) => void): this;
        on(event: "data", listener: (image: Matrix) => void): this;
        on(event: "end", listener: () => void): this;
    }

    export const FACE_CASCADE: string;
    export const EYE_CASCADE: string;
    export const EYEGLASSES_CASCADE: string;
    export const FULLBODY_CASCADE: string;
    export const CAR_SIDE_CASCADE: string;
}
